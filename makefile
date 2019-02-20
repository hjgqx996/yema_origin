#--------------------------------------------------------------
#               Quectel OpenLinux
#   
#--------------------------------------------------------------
SDK_PATH=/home/chenfang/Tbox/1025/ql-ol-sdk/ql-ol-extsdk/
GCC_VERSION=4.9.2

#---------------------
# Include definition
#---------------------
INC_DIR=-I $(SDK_PATH)/ \
		-I $(SDK_PATH)/include \
		-I $(SDK_PATH)/interface/inc \
        -I $(COMPILER_ROOT)/mdm9607/usr/include \
        -I $(SDK_PATH)/include \
        -I $(SDK_PATH)/lib/interface/inc \
        -I ./\
        -I ./YEMA_proto/out_put/\
	    -I$(SDKTARGETSYSROOT)/usr/include               \
	    -I$(SDKTARGETSYSROOT)/usr/include               \
	    -I$(SDKTARGETSYSROOT)/usr/include/data          \
	    -I$(SDKTARGETSYSROOT)/usr/include/dsutils       \
	    -I$(SDKTARGETSYSROOT)/usr/include/qmi           \
	    -I$(SDKTARGETSYSROOT)/usr/include/qmi-framework \

#---------------------
# Source code files
#---------------------
LOCAL_SRC_FILES = \
		./audio.c \
		./gps.c\
		./general.c\
		./sql.cpp\
		./http.c\
		./YEMA_proto/out_put/*.cc\
		./network.cpp\
		./mcu.cpp\
		./gpio_eint.c\
		./main.cpp\
		./cJSON.c\
		./t_ssl.c\
		./ql_tzapp.c\
		./gpExtendQueue.cpp\
		./shellUpdate.cpp\
		./HU.c		

STD_LIB= $(SDKTARGETSYSROOT)/usr/lib/libdsi_netctrl.so     \
         $(SDKTARGETSYSROOT)/usr/lib/libdsutils.so         \
         $(SDKTARGETSYSROOT)/usr/lib/libqmiservices.so     \
         $(SDKTARGETSYSROOT)/usr/lib/libqmi_cci.so         \
         $(SDKTARGETSYSROOT)/usr/lib/libqmi_common_so.so   \
         $(SDKTARGETSYSROOT)/usr/lib/libqmi.so             \
         $(SDKTARGETSYSROOT)/usr/lib/libmcm.so             \
         $(SDKTARGETSYSROOT)/usr/lib/libcurl.so          \
         $(SDKTARGETSYSROOT)/usr/lib/libprotobuf.so           \
         $(SDKTARGETSYSROOT)/usr/lib/libm.so	

#---------------------
# Compilation options
#---------------------
LOCAL_C_FLAGS = -g -Wall -Wno-unused-function -pthread -lsqlite3 -L./ -L$(SDK_PATH)/lib -L$(SDKTARGETSYSROOT)/usr/lib \
				-lrt -lcrypto -lssl\
				-lalsa_intf -lql_lib_audio -lql_common_api -lql_qcmap_client -lcurl -lprotobuf -lstdc++


tbox: clean
	$(CC) $(VERSION_FLAGS) $(LOCAL_C_FLAGS) $(INC_DIR) $(LOCAL_SRC_FILES) $(SDK_PATH)/lib/libql_common_api.a $(SDK_PATH)/lib/libql_peripheral.a $(STD_LIB) -g -s -o $@ -lql_tts_client 


clean:
	rm -rf  *~ *.o

