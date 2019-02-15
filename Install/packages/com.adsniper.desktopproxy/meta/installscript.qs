function Component()
{
    // default constructor
	installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
}

function Controller()
{
}

/* Component.prototype.beginInstallation = function()
{
    if (installer.value("os") === "win") {
        installer.setValue("RunProgram", "@TargetDir@/MetaGate.exe");
    }
}
 */
Component.prototype.createOperations = function()
{
    // call default implementation to actually install MetaGate.exe
    component.createOperations();
	component.addElevatedOperation("Execute", "@TargetDir@\\asdesktopproxyservice.exe", "-i", "UNDOEXECUTE", "@TargetDir@\\asdesktopproxyservice.exe", "-u");
    component.addElevatedOperation("Execute", "@TargetDir@\\asdesktopproxyservice.exe", "", "UNDOEXECUTE", "@TargetDir@\\asdesktopproxyservice.exe", "-t");
}
/*
Component.prototype.IntroductionPageCallback =function() {
 var result = QMessageBox.question("quit.question", "Start Program", "Do you want to start the installed application?",QMessageBox.Yes | QMessageBox.No);
   // if (installer.isUpdater()) {
         if (installer.isProcessRunning("MetaGate.exe")) {
             var widget =gui.currentPageWidget();
             widget.ErrorLabel.setText("<font color='red'>" + "Process: " + "YOUR_PROCESS" + " still running." + "</font>");

            // hide all following pages
installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
}
    // }
}
*/

/*
function Controller()
{
    installer.installationFinished.connect(function() {

        var isUpdate = installer.isUpdater();

        if(isUpdate)
        {
             var targetDir = installer.value("TargetDir");
             console.log("targetDir: " + targetDir);
             //installer.executeDetached(targetDir+"/AutoUpdater.exe --install");

        }else{
            var result = QMessageBox.question("quit.question", "Start Program", "Do you want to start the installed application?",QMessageBox.Yes | QMessageBox.No);
            if( result == QMessageBox.Yes)
            {
                var targetDir = installer.value("TargetDir");
                console.log("targetDir: " + targetDir);
                console.log("Is Updater: " + installer.isUpdater());
                console.log("Is Uninstaller: " + installer.isUninstaller());
                console.log("Is Package Manager: " + installer.isPackageManager());
                //installer.executeDetached(targetDir+"/AutoUpdater.exe --install");
            }
        }
        });
    installer.updateFinished.connect(function(){
            var targetDir = installer.value("TargetDir");
            console.log("targetDir: " + targetDir);
            //installer.executeDetached(targetDir+"/AutoUpdater.exe --install");
    });
}
*/
