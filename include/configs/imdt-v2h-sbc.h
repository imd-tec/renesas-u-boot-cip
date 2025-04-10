#ifndef __IMDT_V2H_H
#define __IMDT_V2H_H

#include "imdt-v2.h"

#if defined(CONFIG_RZ_FEATURES_OPENCVA)
	#define OCA_FEATURE \
 	"ocaaddr=0xC0000000 \0"     \
 	"ocabin=OpenCV_Bin.bin \0"  
 	#define LOAD_COMMAND_OPENCVA "ext4load mmc ${boot_device}:${mmcpart} ${ocaaddr} boot/${ocabin}; "
#endif

#if defined(CONFIG_RZ_FEATURES_CODECS)
	#define CODECS_FEATURE \
		"codaddr=0xC7D00000 \0"     \
		"codbin=Codec_Bin.bin \0"
	#define LOAD_COMMAND_CODEC "ext4load mmc ${boot_device}:${mmcpart} ${codaddr} boot/${codbin}; "
#endif

#endif /* __IMDT_V2H_H */
