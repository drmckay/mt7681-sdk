
IoTManager_custemor_20141615.rar --- Project compiled in Android SDK
	1. put IoTmanager folder to packages/apps on android codebase.
	2. cd [android root]/packages/apps and use mm to make it
	3. install IoTmanager.apk on android device 
	4. put IoTManager/lib/libSmartconnection.so to system/lib at android device that the IoTManager.apk installed


IoTManager_Eclipse.rar   --- Project compiled in Eclipse
	1: extract IoTManager_Eclipse.rar
	2: operate: File-> import -> Android --> Existing Android Code Into Workspace, in Eclipse to import Project
  3: operate: Project-> Build All   ,  to compile project and generate  "IoTManager.apk"
  4: install "IoTmanager.apk" on android device,   but not need adb push libsmartconnection.so
