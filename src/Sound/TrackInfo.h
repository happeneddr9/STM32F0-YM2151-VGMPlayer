/**
 * @Name    : 	TrackInfo.h
 * @Date    : 	2020年1月14日 下午8:18:39
 * @Author  : 	happe
 * @brief   :	here defined the VGM file's header data struct, and the GD3 tracks information.
 *
 */

#ifndef SOUND_TRACKINFO_H_
#define SOUND_TRACKINFO_H_

#include "Arduino/Basic.h"

namespace VGM_info {

	constexpr const char * VGMHeader_InfoString[] = {
			"Indent",
			"EoF",
			"Version",
			"SN76489",

			"YM2413",
			"GD3",
			"Total # samples",
			"Loop offset",

			"Loop # samples",
			"Rate",
			"SN FB",
			"YM2612",

			"YM2151",
			"VGM data offset",
			"SegaPCM",
			"SPCM",

			"RF5C68",
			"YM2203",
			"YM2608",
			"YM2610",

			"YM3812",
			"YM3526",
			"Y8950",
			"YMF262",

			"YMF278B",
			"YMF271",
			"YMZ280B",
			"RF5C164",

			"PWM",
			"AY8910",
			"AYT AY Flags",
			"Volume Modifier",

			"GameBoy DMG",
			"RP2A03",
			"MultiPCM",
			"uPD7759",

			"OkiM6258",
			"Oki Flags",
			"OkiM6295",
			"K051649",

			"K054539",
			"HuC6280",
			"C140",
			"K053260",

			"Pokey",
			"QSound",
			"SCSP",
			"Extra Hdr Ofs",

			"WonderSwan",
			"VSU",
			"SAA1099"
			// ... Coming Soon
	};

	template<size_t N>
	struct SoundChipsName_maker {
			constexpr SoundChipsName_maker(const char * const* header, const size_t idx[])
				: Chips() {
				for (size_t i = 0; i != N; ++i)
					Chips[i] = header[idx[i]];
			}
			const char * Chips[N];
	};

	constexpr const size_t SoundChipsName_index[] = {
			3,
			4,
			11,
			12, 14,
			16, 17, 18, 19,
			20, 21, 22, 23,
			24, 25, 26, 27,
			28, 29,
			32, 33, 34, 35,
			36, 38, 39,
			40, 41, 42, 43,
			44, 45, 46,
			48, 49, 50
	};

	const SoundChipsName_maker<sizeof(SoundChipsName_index) / sizeof(size_t)>
	SoundChipsName(VGMHeader_InfoString, SoundChipsName_index);

	/* VGM File's header */
	struct VGMHeader {
			/* Version V1.00 */
			uint32_t indent;					// Offset: 0x00
			uint32_t EoF;						// Offset: 0x04
			uint32_t version;					// Offset: 0x08
			uint32_t sn76489Clock;				// Offset: 0x0C
			uint32_t ym2413Clock;				// Offset: 0x10
			uint32_t gd3Offset;					// Offset: 0x14
			uint32_t totalSamples;				// Offset: 0x18
			uint32_t loopOffset;				// Offset: 0x1C
			uint32_t loopNumSamples;			// Offset: 0x20
			/* Version V1.01 */
			uint32_t rate;						// Offset: 0x24
			/* Version V1.10 */
			uint32_t snX;						// Offset: 0x28
			uint32_t ym2612Clock;				// Offset: 0x2C
			uint32_t ym2151Clock;				// Offset: 0x30
			/* Version V1.50 */
			uint32_t vgmDataOffset;				// Offset: 0x34
			/* Version V1.51 */
			uint32_t segaPCMClock;				// Offset: 0x38
			uint32_t spcmInterface;				// Offset: 0x3C
			uint32_t rf5C68clock;				// Offset: 0x40
			uint32_t ym2203clock;				// Offset: 0x44
			uint32_t ym2608clock;				// Offset: 0x48
			uint32_t ym2610clock;				// Offset: 0x4C
			uint32_t ym3812clock;				// Offset: 0x50
			uint32_t ym3526clock;				// Offset: 0x54
			uint32_t y8950clock;				// Offset: 0x58
			uint32_t ymf262clock;				// Offset: 0x5C
			uint32_t ymf278bclock;				// Offset: 0x60
			uint32_t ymf271clock;				// Offset: 0x64
			uint32_t ymz280Bclock;				// Offset: 0x68
			uint32_t rf5C164clock;				// Offset: 0x6C
			uint32_t pwmclock;					// Offset: 0x70
			uint32_t ay8910clock;				// Offset: 0x74
			uint32_t ayclockflags;				// Offset: 0x78
			/* Version V1.60 */
			uint32_t vmlblm;					// Offset: 0x7C
			/* Version V1.61 */
			uint32_t gbdgmclock;				// Offset: 0x80
			uint32_t nesapuclock;				// Offset: 0x84
			uint32_t multipcmclock;				// Offset: 0x88
			uint32_t upd7759clock;				// Offset: 0x8C
			uint32_t okim6258clock;				// Offset: 0x90
			uint32_t ofkfcf;					// Offset: 0x94
			uint32_t okim6295clock;				// Offset: 0x98
			uint32_t k051649clock;				// Offset: 0x9C
			uint32_t k054539clock;				// Offset: 0xA0
			uint32_t huc6280clock;				// Offset: 0xA4
			uint32_t c140clock;					// Offset: 0xA8
			uint32_t k053260clock;				// Offset: 0xAC
			uint32_t pokeyclock;				// Offset: 0xB0
			uint32_t qsoundclock;				// Offset: 0xB4
			/* Version V1.71 */
			uint32_t scspclock;					// Offset: 0xB8
			/* Version V1.70 */
			uint32_t extrahdrofs;				// Offset: 0xBC
			/* Version V1.71 */
			uint32_t wonderswanclock;			// Offset: 0xC0
			uint32_t vsuClock;					// Offset: 0xC4
			uint32_t saa1099clock;				// Offset: 0xC8
			// ... Coming Soon

			/* Total 204 Bytes */

			void reset() {		// set all back to zeros
				memset(this, 0, sizeof(*this));
			}

			uint8_t& operator [](size_t n) {
				return *(reinterpret_cast<uint8_t *>(this) + n);
			}

			void chips_usage(Print& os) {
				os << "Chips:";
				for (const size_t& n : SoundChipsName_index) {
					if (reinterpret_cast<uint32_t *>(this)[n] != 0) {
						os << ' ' << SoundChipsName.Chips[(&n - SoundChipsName_index)];
					}
				}
			}

	};

	struct GD3 {
			uint32_t size;
			happe::String enTrackName;
			happe::String enGameName;
			happe::String enSystemName;
			happe::String enAuthor;

			/*happe::String jpTrackName;
			happe::String jpGameName;
			happe::String jpSystemName;
			happe::String jpAuthor;*/

			happe::String releaseDate;

			void reset() {			// clear all
				size = 0;
				enTrackName = "";
				enGameName = "";
				enSystemName = "";
				enAuthor = "";
				releaseDate = "";
			}
	};
}


#endif /* SOUND_TRACKINFO_H_ */
