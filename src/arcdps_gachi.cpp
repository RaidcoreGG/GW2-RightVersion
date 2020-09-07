#pragma comment(lib, "winmm.lib")
#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include <D3D9.h>
#include "arcdps_structs.h"

/* proto/globals */
arcdps_exports arc_exports;
char* arcvers;
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversionstr, void* imguicontext, IDirect3DDevice9* id3dd9);
extern "C" __declspec(dllexport) void* get_release_addr();
arcdps_exports* mod_init();
uintptr_t mod_release();
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname);
uintptr_t mod_combat_local(cbtevent* ev, ag* src, ag* dst, char* skillname);


/* export -- arcdps looks for this exported function and calls the address it returns on client load */
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversionstr, void* imguicontext, IDirect3DDevice9* id3dd9)
{
	arcvers = arcversionstr;
	//ImGui::SetCurrentContext((ImGuiContext*)imguicontext);
	return mod_init;
}

/* export -- arcdps looks for this exported function and calls the address it returns on client exit */
extern "C" __declspec(dllexport) void* get_release_addr()
{
	arcvers = 0;
	return mod_release;
}

/* initialize mod -- return table that arcdps will use for callbacks */
arcdps_exports* mod_init()
{
	/* for arcdps */
	memset(&arc_exports, 0, sizeof(arcdps_exports));
	arc_exports.sig = 0x138B8ADD;
	arc_exports.size = sizeof(arcdps_exports);
	arc_exports.out_name = "Gachi (by DeltaxHunter)";
	arc_exports.out_build = "2020-09-06";
	arc_exports.wnd_nofilter = mod_wnd;
	arc_exports.combat = mod_combat;
	//arc_exports.combat_local = mod_combat_local;
	//arc_exports.size = (uintptr_t)"error message if you decide to not load, sig must be 0";

	return &arc_exports;
}

/* release mod -- return ignored */
uintptr_t mod_release()
{
	return 0;
}

/* window callback -- return is assigned to umsg (return zero to not be processed by arcdps or game) */
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return uMsg;
}

/* skill ids*/
const uint16_t DHUUM_SUCTION = 48210;
const uint16_t ENSOLYSS_SUCTION = 37045;
const uint16_t ASSAULT_KNIGHT_SUCTION = 36903;
const uint16_t ENDERS_ECHO_PICKUP = 47076;
const uint16_t SABETHA_FIRESTORM = 31332;
const uint16_t THROW_OIL = 34471;
const uint16_t RAPID_DECAY = 37716;
const uint16_t CA_PULVERIZE = 52173;
const uint16_t CA_TREMOR = 52656;
const uint16_t CA_THUNDERCLAP = 52843;
const uint16_t CA_INVULN = 52255;
//const uint16_t FIREBALL = 5491;
const uint16_t HEADBUTT = 30343;

/* combat callback -- may be called asynchronously. return ignored */
/* one participant will be party/squad, or minion of. no spawn statechange events. despawn statechange only on marked boss npcs */
uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname) {
	if (ev)
	{
		if (!ev->dst_agent && ev->is_activation == 1) // when skill has target and is being activated
		{
			switch (ev->skillid)
			{
			case DHUUM_SUCTION:
				sndPlaySound(L"suction.wav", SND_FILENAME);
				break;
			case ENSOLYSS_SUCTION:
				sndPlaySound(L"suction.wav", SND_FILENAME);
				break;
			case ASSAULT_KNIGHT_SUCTION:
				sndPlaySound(L"suction.wav", SND_FILENAME);
				break;
			case SABETHA_FIRESTORM:
				sndPlaySound(L"ILoveFire.wav", SND_FILENAME);
				break;
			case THROW_OIL:
				sndPlaySound(L"MaybeALittleOil.wav", SND_FILENAME);
				break;
			}
		}
		else
		{
			switch (ev->skillid)
			{
			case CA_PULVERIZE:
				sndPlaySound(L"fisting.wav", SND_FILENAME);
				break;
			case CA_TREMOR:
				sndPlaySound(L"fisting.wav", SND_FILENAME);
				break;
			case CA_THUNDERCLAP:
				sndPlaySound(L"spank.wav", SND_FILENAME);
				break;
			case RAPID_DECAY:
				sndPlaySound(L"WhatTheHellAreYouDoing.wav", SND_FILENAME);
				break;
			case CA_INVULN:
				if (ev->is_buffremove) // when invuln buff is removed
				{
					sndPlaySound(L"OhMyShoulder.wav", SND_FILENAME);
				}
				break;
			case HEADBUTT:
				sndPlaySound(L"GetYourAssDown.wav", SND_FILENAME);
				break;
			}
		}

		/*if ((ev->skillid == CA_PULVERIZE || ev->skillid == CA_TREMOR)) {
			if (ev->time - lastPulverize >= 5000) {
				lastPulverize = ev->time;
				sndPlaySound(L"fisting.wav", SND_FILENAME);
			}
		}
		else if (ev->skillid == CA_THUNDERCLAP) {
			if (ev->time - lastThunderclap >= 5000) {
				lastThunderclap = ev->time;
				sndPlaySound(L"spank.wav", SND_FILENAME);
			}
		}
		else if (ev->skillid == RAPID_DECAY && ev->value >= 1000) {
			if (ev->time - lastRapidDecay >= 6000) {
				lastRapidDecay = ev->time;
				sndPlaySound(L"WhatTheHellAreYouDoing.wav", SND_FILENAME);
			}
		}*/
		/*if (ev->skillid == ENDERS_ECHO_PICKUP) {
			uint64_t timeNow = ev->time;
			uint64_t lastPickup = GetWithDef(lastPickedUpTime, ev->dst_agent, (uint64_t)0);

			lastPickedUpTime[ev->dst_agent] = timeNow;

			if (timeNow - lastPickup > 1000) {
				sndPlaySound(L"TakeMeBoy.wav", SND_FILENAME);
			}
		}
		else if (ev->is_statechange == 11) { //weapon swap
			// when someone gets released from Ender Echo, weapon swap is triggered

			uint64_t timeNow = ev->time;
			uint64_t lastPickup = GetWithDef(lastPickedUpTime, ev->src_agent, (uint64_t)0);

			if (timeNow - lastPickup <= 1000) { //make sure that the player was picked before, and that it wasn't a normal weapon swap
				// we don't want to play the "get your ass back here" sound when Ender's Echo kills the captured player
				// so we check after 100ms if the player is dead
				playGetYourAssBackHereAt = timeNow + 200;
			}
		}
		else if (ev->is_statechange == 4) { //death
			// if someone dies, do not play the "get your ass back here" sound, because it was probably the person that was captured
			playGetYourAssBackHereAt = 0;
		}

		if (playGetYourAssBackHereAt != 0 && ev->time >= playGetYourAssBackHereAt) {
			playGetYourAssBackHereAt = 0;
			sndPlaySound(L"GetYourAssBackHere.wav", SND_FILENAME);
		}*/
	}

	return 0;
}