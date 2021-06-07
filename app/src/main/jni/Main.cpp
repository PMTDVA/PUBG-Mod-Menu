/*
 * Credits:
 *
 * Octowolve - Mod menu: https://github.com/z3r0Sec/Substrate-Template-With-Mod-Menu
 * And hooking: https://github.com/z3r0Sec/Substrate-Hooking-Example
 * VanHoevenTR A.K.A Nixi: https://github.com/LGLTeam/VanHoevenTR_Android_Mod_Menu
 * MrIkso - Mod menu: https://github.com/MrIkso/FloatingModMenu
 * Rprop - https://github.com/Rprop/And64InlineHook
 * MJx0 A.K.A Ruit - KittyMemory: https://github.com/MJx0/KittyMemory
 * */
#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Includes/obfuscate.h"

#include "Menu.h"

#include "Toast.h"

#if defined(__aarch64__) //Compile for arm64 lib only
#include <And64InlineHook/And64InlineHook.hpp>
#else //Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works

#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>

#endif

// fancy struct for patches for kittyMemory
struct My_Patches {
    // let's assume we have patches for these functions for whatever game
    // like show in miniMap boolean function
    MemoryPatch a,b,b1,b2,c,d,e,e0,e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,f,g,h,i,j;
    // etc...
} hexPatches;

bool feature1 = false, feature2 = false, feature3 = false, feature4 = false,  feature5 = false, feature6 = false, feature7 = false, feature8 = false,  feature9 = false, feature10 = false, featureHookToggle = false;
void *instanceBtn;

// Function pointer splitted because we want to avoid crash when the il2cpp lib isn't loaded.
// If you putted getAbsoluteAddress here, the lib tries to read the address without il2cpp loaded,
// will result in a null pointer which will cause crash
// See https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
void (*AddMoneyExample)(void *instance, int amount);

//Target lib here
#define targetLibName OBFUSCATE("")

extern "C" {
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint feature, jint value, jboolean boolean, jstring str) {

    const char *featureName = env->GetStringUTFChars(str, 0);
    feature += 1;  // No need to count from 0 anymore. yaaay :)))

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d"), feature, featureName, value,
         boolean);

    //!!! BE CAREFUL NOT TO ACCIDENTLY REMOVE break; !!!//

//ini Biarin Aja
    if (feature == 1) {
        feature1 = boolean;
        if (feature1) {
            hexPatches.a.Modify();

        } else {
            hexPatches.a.Restore();
        }
    }

    else if (feature == 2) {
        feature2 = boolean;
        if (feature2) {
            hexPatches.b.Modify();
            hexPatches.b1.Modify();
            hexPatches.b2.Modify();

        } else {
            hexPatches.b.Restore();
            hexPatches.b1.Restore();
            hexPatches.b2.Restore();
        }
    }

    else if (feature == 3) {
        feature3 = boolean;
        if (feature3) {
            hexPatches.c.Modify();

        } else {
            hexPatches.c.Restore();
        }
    }

    else if (feature == 4) {
        feature4 = boolean;
        if (feature4) {
            hexPatches.d.Modify();

        } else {
            hexPatches.d.Restore();
        }
    }

        else if (feature == 5) {
        if (value == 0) {
            hexPatches.e0.Restore();
        } else if (value == 1) {
            hexPatches.e1.Modify();
        } else if (value == 2) {
            hexPatches.e2.Modify();
        } else if (value == 3) {
            hexPatches.e3.Modify();
        } else if (value == 4) {
            hexPatches.e4.Modify();
        } else if (value == 5) {
            hexPatches.e5.Modify();
        } else if (value == 6) {
            hexPatches.e6.Modify();
        } else if (value == 7) {
            hexPatches.e7.Modify();
        } else if (value == 8) {
            hexPatches.e8.Modify();
        } else if (value == 9) {
            hexPatches.e9.Modify();
        } else if (value == 10) {
            hexPatches.e10.Modify();
        }
    }
    
    else if (feature == 6) {
        feature6 = boolean;
        if (feature6) {
            hexPatches.f.Modify();

        } else {
            hexPatches.f.Restore();
        }
    }

    else if (feature == 7) {
        feature7 = boolean;
        if (feature7) {
            hexPatches.g.Modify();

        } else {
            hexPatches.g.Restore();
        }
    }
    
    else if (feature == 8) {
        feature8 = boolean;
        if (feature8) {
            hexPatches.h.Modify();

        } else {
            hexPatches.h.Restore();
        }
    }

    else if (feature == 9) {
        feature9 = boolean;
        if (feature9) {
            hexPatches.i.Modify();

        } else {
            hexPatches.i.Restore();
        }
    }

    else if (feature == 10) {
        feature10 = boolean;
        if (feature10) {
            hexPatches.j.Modify();

        } else {
            hexPatches.j.Restore();
        }
    }


}
}

bool (*old_get_BoolExample)(void *instance);
bool get_BoolExample(void *instance) {
    if (instance != NULL && featureHookToggle) {
        return true;
    }
    return old_get_BoolExample(instance);
}
void (*old_Update)(void *instance);
void Update(void *instance) {
    instanceBtn = instance;
    old_Update(instance);
}

float (*old_get_FloatExample)(void *instance);

void *hack_thread(void *) {
	//ProcMap il2cppMap;
	ProcMap libtersafe;
	do {
		//il2cppMap = KittyMemory::getLibraryMap("libUE4.so");
		libtersafe = KittyMemory::getLibraryMap("libtersafe.so");
		sleep(1);
	} while(!libtersafe.isValid());
    sleep(1);

    hexPatches.a = MemoryPatch::createWithHex("libUE4.so", 0x136D4F8, "00 00 00 00");
    //hexPatches.bypass = MemoryPatch::createWithHex("libUE4.so", 0x212D900, "00 00 A0 E3 1E FF 2F E1");
    //hexPatches.bypass2 = MemoryPatch::createWithHex("libtersafe.so", 0x529368, "00 00 00 00 06 98 00 28");
    //hexPatches.bypass3 = MemoryPatch::createWithHex("libtersafe.so", 0x530780, "00 00 00 00 29 98 00 28");

    hexPatches.b = MemoryPatch::createWithHex("libUE4.so", 0x258B740, "01 00 00 7A");
    hexPatches.b1 = MemoryPatch::createWithHex("libUE4.so", 0x258B880, "00 00 00 00");
    hexPatches.b2 = MemoryPatch::createWithHex("libUE4.so", 0x258B74C, "00 00 00 00");
    //hexPatches.nc = MemoryPatch::createWithHex("libUE4.so", 0x372D818, "00 00 00 00");
    //hexPatches.nc2 = MemoryPatch::createWithHex("libUE4.so", 0x367EC84, "00 00 00 00");

    //hexPatches.c = MemoryPatch::createWithHex("libUE4.so", 0x258B74C, "00 00 00 00");
    hexPatches.c = MemoryPatch::createWithHex("libUE4.so", 0x3C491D0, "00 00 20 42");

    //hexPatches.inshit = MemoryPatch::createWithHex("libUE4.so", 0x1D7EDB0, "99 F0 20 E3 1E FF 2F E1");

    //hexPatches.bt = MemoryPatch::createWithHex("libUE4.so", 0x3906258, "00 00 20 42");

    //hexPatches.mbvl = MemoryPatch::createWithHex("libUE4.so", 0x3B64788, "00 00 F0 41");
    //hexPatches.mbl = MemoryPatch::createWithHex("libUE4.so", 0x3B64788, "00 00 20 42");
    //hexPatches.d = MemoryPatch::createWithHex("libUE4.so", 0x3C491D0, "00 00 70 42");
    //hexPatches.mbh = MemoryPatch::createWithHex("libUE4.so", 0x3B64788, "00 00 70 42");
    //hexPatches.mbvh = MemoryPatch::createWithHex("libUE4.so", 0x3B64788, "00 00 8C 42");

    hexPatches.d = MemoryPatch::createWithHex("libUE4.so", 0x1C113E8, "01 00 00 00 10 0A 10 EE");
    //hexPatches.ee = MemoryPatch::createWithHex("libUE4.so", 0xFB2E54, "01 00 00 7A");
    //hexPatches.eee = MemoryPatch::createWithHex("libUE4.so", 0x24A74BC, "02 50 A0 E1");

    hexPatches.e0 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 B4 43");
    hexPatches.e1 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 AA 43");
    hexPatches.e2 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 A0 43");
    hexPatches.e3 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 96 43");
    hexPatches.e4 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 8C 43");
    hexPatches.e5 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 82 43");
    hexPatches.e6 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 70 43");
    hexPatches.e7 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 5C 43");
    hexPatches.e8 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 52 43");
    hexPatches.e9 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 48 43");
    hexPatches.e10 = MemoryPatch::createWithHex("libUE4.so", 0x381FCB0, "00 00 3E 43");
    //hexPatches.ff = MemoryPatch::createWithHex("libUE4.so", 0x24A74BC, "00 00 00 00");

    hexPatches.f = MemoryPatch::createWithHex("libUE4.so", 0x2D96C18, "00 00 60 41");
    hexPatches.g = MemoryPatch::createWithHex("libUE4.so", 0x2D0DA34, "00 00 00 00");
    hexPatches.h = MemoryPatch::createWithHex("libUE4.so", 0x3F109B0, "00 00 00 00");
    hexPatches.i = MemoryPatch::createWithHex("libUE4.so", 0x3AD36A0, "B0 C6 27 B7 00 F0 20 E3");
    hexPatches.j = MemoryPatch::createWithHex("libUE4.so", 0x119BE24, "02 1A B7 EE");
    
    //hexPatches.g5 = MemoryPatch::createWithHex("libUE4.so", 0x37307E0, "00 00 82 43");
    //hexPatches.g6 = MemoryPatch::createWithHex("libUE4.so", 0x37307E0, "00 00 70 43");
    //hexPatches.g7 = MemoryPatch::createWithHex("libUE4.so", 0x37307E0, "00 00 5C 43");
    //hexPatches.g8 = MemoryPatch::createWithHex("libUE4.so", 0x37307E0, "00 00 52 43");
    //hexPatches.g9 = MemoryPatch::createWithHex("libUE4.so", 0x37307E0, "00 00 48 43");
    //hexPatches.g10 = MemoryPatch::createWithHex("libUE4.so", 0x37307E0, "00 00 3E 43");

    //hexPatches.h = MemoryPatch::createWithHex("libUE4.so", 0x2C344C8, "00 00 00 00");

    //hexPatches.i = MemoryPatch::createWithHex("libUE4.so", 0x2CBD788, "00 00 60 41");

    //hexPatches.bs = MemoryPatch::createWithHex("libUE4.so", 0x3997244, "00 00 00 00");

    //hexPatches.dm = MemoryPatch::createWithHex("libUE4.so", 0x2E082FC, "41 2A B1 EE");

    //hexPatches.j = MemoryPatch::createWithHex("libUE4.so", 0x1150134, "02 1A B7 EE");

    //hexPatches.flash = MemoryPatch::createWithHex("libUE4.so", 0x39D9D8C, "00 00 00 00");
    //hexPatches.k = MemoryPatch::createWithHex("libUE4.so", 0x1D40C98, "00 00 31 36 ");
    //hexPatches.k1 = MemoryPatch::createWithHex("libUE4.so", 0x1C57DEC, "00 00 31 36 ");
    //hexPatches.k2 = MemoryPatch::createWithHex("libUE4.so", 0x1DBA718, "00 00 31 36 ");
    //hexPatches.k3 = MemoryPatch::createWithHex("libUE4.so", 0x4C4325, "00 00 31 36 ");

    //hexPatches.grass = MemoryPatch::createWithHex("libUE4.so", 0x2475D58, "00 00 00 00");




    /* hexPatches.Fiture4 = MemoryPatch::createWithHex("libUE4.so", 0x12281E4, "00 00 C0 41");

     hexPatches.Fiture5 = MemoryPatch::createWithHex("libUE4.so", 0x2125E0C, "00 00 00 00");
     hexPatches.Fiture6 = MemoryPatch::createWithHex("libUE4.so", 0x4BC0560, "00 00 00 00");
     hexPatches.Fiture7 = MemoryPatch::createWithHex("libUE4.so", 0x22BF3BC, "00 00 00 00");

     hexPatches.NotXColorsBlack = MemoryPatch::createWithHex("libUE4.so", 0x2ACCC28, "00 00 60 41");

     hexPatches.Fiture9 = MemoryPatch::createWithHex("libUE4.so", 0x2A43A18, "42 0A 30 EE");

     hexPatches.Fiture10= MemoryPatch::createWithHex("libUE4.so", 0x2BC492C, "00 00 00 00");



     hexPatches.wideview= MemoryPatch::createWithHex("libUE4.so", 0x34D7E30, "00 00 72 43");
     */


    return NULL;
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
    pthread_t p;
    pthread_create(&p, NULL, antiLeech, NULL);
}
