#ifndef TRANSACTIONSDBSTORAGE_H
#define TRANSACTIONSDBSTORAGE_H

#include "dbstorage.h"
#include "Transaction.h"
#include "BigNumber.h"
#include <vector>

#include <QSqlQuery>
namespace transactions {

class TransactionsDBStorage : public DBStorage
{
public:
    TransactionsDBStorage(const QString &path = QString(), QObject *parent = nullptr);

    virtual int currentVersion() const final;

    void addPayment(const QString &currency, const QString &txid, const QString &address, bool isInput,
                    const QString &ufrom, const QString &uto, const QString &value,
                    quint64 ts, const QString &data, const QString &fee, qint64 nonce,
                    bool isSetDelegate, bool isDelegate, const QString &delegateValue, const QString &delegateHash,
                    Transaction::Status status, Transaction::Type type, qint64 blockNumber);
    void addPaymentV2(const QString &currency, const QString &txid, const QString &address, bool isInput,
                    const QString &ufrom, const QString &uto, const QString &value,
                    quint64 ts, const QString &data, const QString &fee, qint64 nonce,
                    bool isSetDelegate, bool isDelegate, QString delegateValue, const QString &delegateHash,
                      Transaction::Status status, Transaction::Type type, qint64 blockNumber);

    void addPayment(const Transaction &trans);
    void addPayments(const std::vector<Transaction> &transactions);

    std::vector<Transaction> getPaymentsForAddress(const QString &address, const QString &currency,
                                              qint64 offset, qint64 count, bool asc);
    std::vector<Transaction> getPaymentsForCurrency(const QString &currency,
                                                  qint64 offset, qint64 count, bool asc) const;

    std::vector<Transaction> getPaymentsForAddressPending(const QString &address, const QString &currency,
                                                            bool asc) const;

    std::vector<Transaction> getForgingPaymentsForAddress(const QString &address, const QString &currency,
                                              qint64 offset, qint64 count, bool asc);

    Transaction getLastPaymentIsSetDelegate(const QString &address, const QString &currency,
                                            const QString &from, const QString &to,
                                            bool isInput, bool isDelegate);

    Transaction getLastForgingTransaction(const QString &address, const QString &currency);

    void updatePayment(const QString &address, const QString &currency, const QString &txid, bool isInput, const Transaction &trans);
    void removePaymentsForDest(const QString &address, const QString &currency);

    qint64 getPaymentsCountForAddress(const QString &address, const QString &currency, bool input);

    BigNumber calcInValueForAddress(const QString &address, const QString &currency);
    BigNumber calcOutValueForAddress(const QString &address, const QString &currency);

    qint64 getIsSetDelegatePaymentsCountForAddress(const QString &address, const QString &currency, Transaction::Status status = Transaction::OK);
    BigNumber calcIsSetDelegateValueForAddress(const QString &address, const QString &currency, bool isDelegate, bool isInput, Transaction::Status status = Transaction::OK);

    void calcBalance(const QString &address, const QString &currency,
                     BalanceInfo &balance);

    void addTracked(const QString &currency, const QString &address, const QString &name, const QString &type, const QString &tgroup);
    void addTracked(const AddressInfo &info);
    void addTracked(const std::vector<AddressInfo> &addresses);

    std::vector<AddressInfo> getTrackedForGroup(const QString &tgroup);

    void removePaymentsForCurrency(const QString &currency);

protected:
    virtual void createDatabase() final;

private:
    void setTransactionFromQuery(QSqlQuery &query, Transaction &trans) const;

    void createPaymentsList(QSqlQuery &query, std::vector<Transaction> &payments) const;
    QSqlQuery m_iquery;
};

}

#endif // TRANSACTIONSDBSTORAGE_H
