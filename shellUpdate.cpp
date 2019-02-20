#include "shellUpdate.h"

#define VERSIONSTAMP "/home/root/versionStamp"
#define SHELLSTAMP "/home/root/shellStamp"
#define PROFILETEMP "/home/root/profile"
#define PROFILEPATH "/etc/profile"
#define CHECKSHELL "/etc/checksubregion.sh" 
#define APPUPDATESHELL "/home/root/update.sh"

void writeAppUpdate(){
		if((access(APPUPDATESHELL,F_OK))==0){
			system("rm /home/root/update.sh");
		}
		const char *shellCmd = 
"#!/bin/sh\n\
LOCK_NAME=\"/tmp/tbox_upgrade.lock\"\n\
if ( set -o noclobber; echo \"$$\" > \"$LOCK_NAME\") 2> /dev/null;\n\
then\n\
	trap 'rm -f  \"$LOCK_NAME\"; exit $?' INT TERM EXIT\n\
\n\
### start\n\
#wget -T 30 -t 5 -O /home/root/new_tbox $1\n\
#if [ $? -eq 0 ]\n\
#then\n\
#echo wget ok\n\
#sync\n\
	kill -9 $(pidof tbox)\n\
	mv /home/root/tbox /home/root/old_tbox\n\
	sync\n\
	sleep 1\n\
	mv /home/root/tbox_decrypt /home/root/tbox\n\
	chmod 777 /home/root/tbox\n\
	sync\n\
	echo upgrade success\n\
	reboot\n\
#else\n\
#echo wget failed\n\
#rm new_tbox\n\
#fi\n\
### end\n\
\n\
### Removing lock\n\
	rm -f $LOCK_NAME\n\
	trap - INT TERM EXIT\n\
	else\n\
	echo \"Failed to acquire lockfile: $LOCK_NAME.\"\n\
	echo \"Held by $(cat $LOCK_NAME)\"\n\
	exit 1\n\
fi";	

		FILE *fd = fopen(APPUPDATESHELL,"a");
		fwrite(shellCmd,sizeof(unsigned char),strlen(shellCmd),fd);
		fclose(fd);
		system("sync");	
}

void writeCheckSubregin(){
		if((access(CHECKSHELL,F_OK))==0){
			system("rm /etc/checksubregion.sh");
		}
		const char *shellCmd = 
"#!/bin/sh\n\
sysVer=0\n\
usrVer=0\n\
sysStampFile=\"/home/root/versionStamp\"\n\
usrStampFile=\"/usrdata/versionStamp\"\n\
if [ ! -f $sysStampFile ];then\n\
	sysVer=-1\n\
else\n\
	sysVer=`cat $sysStampFile`\n\
	echo $sysVer\n\
fi\n\
if [ ! -f $usrStampFile ];then\n\
	usrVer=-1\n\
else\n\
	usrVer=`cat $usrStampFile`\n\
fi\n\
	echo $usrVer $sysVer\n\
if [ \"$usrVer\" == -1 -a \"$sysVer\" == -1 ];then\n\
	echo \"sys and user space all have no file\"\n\
	exit 0\n\
fi\n\
	echo \"latest latest\"\n\
if [ \"$usrVer\" -eq \"$sysVer\" ];then\n\
	echo \"sys and user space have the same file\"\n\
	exit 0\n\
fi\n\
if [ \"$usrVer\" -gt \"$sysVer\" ];then\n\
	echo \"user space have latest file\"\n\
	rm -rf /home/root/*\n\
	sync\n\
	cp -rf /usrdata/* /home/root\n\
	sync\n\
	exit 0\n\
fi\n\
if [ \"$usrVer\" -lt \"$sysVer\" ];then\n\
	echo \"sys space have latest file\"\n\
	rm -rf /usrdata/*\n\
	sync\n\
	cp -rf /home/root/* /usrdata/\n\
	sync\n\
	exit 0\n\
fi";	

		FILE *fd = fopen(CHECKSHELL,"a");
		fwrite(shellCmd,sizeof(unsigned char),strlen(shellCmd),fd);
		fclose(fd);
		system("sync");	
}

void writeProfile(){
		if((access(PROFILETEMP,F_OK))==0){
			system("rm /home/root/profile");
		}
		const char *shellCmd = 
"# /etc/profile: system-wide .profile file for the Bourne shell (sh(1))\n\
# and Bourne compatible shells (bash(1), ksh(1), ash(1), ...).\n\
PATH=\"/usr/local/bin:/usr/bin:/bin\"\n\
EDITOR=\"vi\"					  # needed for packages like cron, git-commit\n\
test -z \"$TERM\" && TERM=\"vt100\" # Basic terminal capab. For screen etc.\n\
\n\
if [ ! -e /etc/localtime -a ! -e /etc/TZ ]; then\n\
	TZ=\"UTC\"				# Time Zone. Look at http://theory.uwinnipeg.ca/gnu/glibc/libc_303.html\n\
# for an explanation of how to set this to your local timezone.\n\
	export TZ\n\
fi\n\
\n\
if [ \"$HOME\" = \"/home/root\" ]; then\n\
	PATH=$PATH:/usr/local/sbin:/usr/sbin:/sbin\n\
fi\n\
if [ \"$PS1\" ]; then\n\
# works for bash and ash (no other shells known to be in use here)\n\
	PS1='\\u@\\h:\\w\\$ '\n\
fi\n\
\n\
if [ -d /etc/profile.d ]; then\n\
for i in /etc/profile.d/* ; do\n\
	. $i\n\
	done\n\
	unset i\n\
fi\n\
\n\
if [ -x /usr/bin/resize ];then\n\
	 /usr/bin/resize >/dev/null\n\
fi\n\
\n\
export PATH PS1 OPIEDIR QPEDIR QTDIR EDITOR TERM\n\
\n\
umask 022\n\
\n\
/etc/checksubregion.sh\n\
/home/root/tbox &";


		FILE *fd = fopen(PROFILETEMP,"a");
		fwrite(shellCmd,sizeof(unsigned char),strlen(shellCmd),fd);
		fclose(fd);
		system("mv /home/root/profile /etc/profile");
		system("rm /home/root");
		system("sync");	
}

int getShellUpdateNum(shellType type){
	char numData[10] = {0};
	int fileSize,len;
	if(type == newShellType){
		FILE *fd = fopen(SHELLSTAMP,"r");
		if(fd){
			fseek(fd, 0L, SEEK_END);  
    		fileSize = ftell(fd);  
			fseek(fd,0L,SEEK_SET);
			if((len = fread(numData,fileSize,1,fd)) > 0){
				return atoi(numData);
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}

	if(type == backupShellType){
	FILE *fd = fopen(VERSIONSTAMP,"r");
	if(fd){
			fseek(fd, 0L, SEEK_END);  
    		fileSize = ftell(fd);  
			fseek(fd,0L,SEEK_SET);
			if((len = fread(numData,fileSize,1,fd)) > 0){
				return atoi(numData);
			}else{
				return 0;
			}
		}
	}else{
			return 0;
	}

	return 0;
}

int addShellUpdateNum(shellType type){
	char numData[10] = {0};
	int fileSize,len,num;
	if(type == newShellType){
		FILE *fd = fopen(SHELLSTAMP,"rw+");
		if(fd){
			fseek(fd, 0L, SEEK_END);  
    		fileSize = ftell(fd);  
			fseek(fd,0L,SEEK_SET);
			if((len = fread(numData,fileSize,1,fd)) > 0){
				num = atoi(numData);
				num++;
				memset(numData,0,sizeof(numData));				
				snprintf(numData, sizeof(numData), "%d", num);
				fseek(fd,0L,SEEK_SET);
				fwrite(numData,strlen(numData),1,fd);
			}else{
				return 0;
			}
			fclose(fd);	
			system("sync");
		}else{
			return 0;
		}
	}

	if(type == backupShellType){
	FILE *fd = fopen(VERSIONSTAMP,"rw+");
	if(fd){
			fseek(fd, 0L, SEEK_END);  
    		fileSize = ftell(fd);  
			fseek(fd,0L,SEEK_SET);
			if((len = fread(numData,fileSize,1,fd)) > 0){
				num = atoi(numData);
				num++;
				memset(numData,0,sizeof(numData));
				snprintf(numData, sizeof(numData), "%d", num);
				fseek(fd,0L,SEEK_SET);			
				fwrite(numData,strlen(numData),1,fd);
			}else{
				return 0;
			}
		}
		fclose(fd);	
		system("sync");
	}else{
			return 0;
	}

	return 0;
}

void updateShell(){	
	if((access(SHELLSTAMP,F_OK))<0 ||getShellUpdateNum(backupShellType) < 1){
		writeProfile();
		writeCheckSubregin();
		writeAppUpdate();
		system("chmod 744 /etc/checksubregion.sh");
		system("chmod 744 /home/root/update.sh");
		system("cp MX_pub.key ca-cert.crt client-cert.crt client-key.pem en.pem update.sh tbox disk.sh param.json param.json.bak  /home/root");
		system("echo \"1\">/home/root/shellStamp");
		system("echo \"1\">/home/root/versionStamp");
		system("reboot -f");
	}
	//system(shellCmd);
}


