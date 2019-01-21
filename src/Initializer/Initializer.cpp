#include "Initializer.h"

#include <QSettings>

#include "check.h"
#include "SlotWrapper.h"
#include "Paths.h"

#include "InitializerJavascript.h"
#include "InitInterface.h"

namespace initializer {

InitState::InitState(const QString &type, const QString &subType, const QString &message, bool isCritical, const TypedException &exception)
    : type(type)
    , subType(subType)
    , message(message)
    , isCritical(isCritical)
    , exception(exception)
{}

Initializer::Initializer(InitializerJavascript &javascriptWrapper, QObject *parent)
    : QObject(parent)
    , javascriptWrapper(javascriptWrapper)
{
    CHECK(connect(this, &Initializer::resendAllStatesSig, this, &Initializer::onResendAllStates), "not connect onGetAllStates");
    CHECK(connect(this, &Initializer::javascriptReadySig, this, &Initializer::onJavascriptReady), "not connect onJavascriptReady");
    CHECK(connect(this, &Initializer::sendState, this, &Initializer::onSendState, Qt::ConnectionType::QueuedConnection), "not connect onSendState");

    qRegisterMetaType<Callback>("Callback");
    qRegisterMetaType<GetAllStatesCallback>("GetAllStatesCallback");
    qRegisterMetaType<ReadyCallback>("ReadyCallback");
    qRegisterMetaType<InitState>("InitState");
}

Initializer::~Initializer() = default;

void Initializer::complete() {
    isComplete = true;
}

template<typename Func>
void Initializer::runCallback(const Func &callback) {
    emit javascriptWrapper.callbackCall(callback);
}

void Initializer::sendStateToJs(const InitState &state, int number, int numberCritical) {
    LOG << "State sended " << number << " " << totalStates << " " << numberCritical << " " << totalCriticalStates << " " << state.type << " " << state.subType << " " << state.exception.numError << " " << state.exception.description;
    emit javascriptWrapper.stateChangedSig(number, totalStates, numberCritical, totalCriticalStates, state);
}

void Initializer::sendInitializedToJs(bool isErrorExist) {
    LOG << "Initialized sended " << !isErrorExist;
    emit javascriptWrapper.initializedSig(!isErrorExist, TypedException());
}

void Initializer::sendCriticalInitializedToJs(bool isErrorExist) {
    LOG << "Critical initialized sended " << !isErrorExist;
    emit javascriptWrapper.initializedCriticalSig(!isErrorExist, TypedException());
}

void Initializer::onSendState(const InitState &state) {
BEGIN_SLOT_WRAPPER
    CHECK(isComplete, "Not complete"); // Запросы приходят в QueuedConnection, поэтому флаг isComplete в этот момент уже должен быть установлен
    CHECK(existStates.size() == states.size(), "Incorrect existStates struct");
    CHECK(existStates.find(std::make_pair(state.type, state.subType)) == existStates.end(), "State already setted");
    CHECK((int)states.size() < totalStates, "Incorrect state number");
    existStates.emplace(state.type, state.subType);
    states.emplace_back(state);
    if (state.exception.isSet()) {
        isErrorExist = true;
        if (state.isCritical) {
            isErrorCritical = true;
        }
    }
    if (state.isCritical) {
        CHECK(countCritical < totalCriticalStates, "Incorrect count critical");
        countCritical++;
    }
    sendStateToJs(state, (int)states.size(), countCritical);
    if (!isCriticalInitFinished && countCritical == totalCriticalStates) {
        isCriticalInitFinished = true;
        sendCriticalInitializedToJs(isErrorCritical);
    }
    if (!isInitFinished && (int)states.size() == totalStates) {
        isInitFinished = true;
        sendInitializedToJs(isErrorExist);
    }
END_SLOT_WRAPPER
}

void Initializer::onResendAllStates(const GetAllStatesCallback &callback) {
BEGIN_SLOT_WRAPPER
    const TypedException exception = apiVrapper2([&, this] {
        if (isComplete) {
            int cCritical = 0;
            for (size_t i = 0; i < states.size(); i++) {
                if (states[i].isCritical) {
                    cCritical++;
                }
                sendStateToJs(states[i], (int)i, cCritical);
            }
            if (isCriticalInitFinished) {
                sendCriticalInitializedToJs(isErrorCritical);
            }
            if (isInitFinished) {
                sendInitializedToJs(isErrorExist);
            }
        }
    });
    runCallback(std::bind(callback, exception));
END_SLOT_WRAPPER
}

void Initializer::onJavascriptReady(bool force, const ReadyCallback &callback) {
BEGIN_SLOT_WRAPPER
    ReadyType result = ReadyType::Error;
    const TypedException exception = apiVrapper2([&, this] {
        if (!isCriticalInitFinished) {
            result = ReadyType::CriticalAdvance;
            return;
        }
        if (!isInitFinished && !force) {
            result = ReadyType::Advance;
            return;
        }
        if (isErrorExist && !force) {
            result = ReadyType::NotSuccess;
            return;
        }
        for (std::unique_ptr<InitInterface> &init: initializiers) {
            init->complete();
        }
        result = ReadyType::Finish;
    });
    runCallback(std::bind(callback, result, exception));
END_SLOT_WRAPPER
}

}