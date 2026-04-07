#ifndef SYMBOLS_H
#define SYMBOLS_H

/* * NOTA: Para estes símbolos funcionarem no Windows, o arquivo renderer.c 
 * deve chamar SetConsoleOutputCP(CP_UTF8) no início da execução.
 */

/* --- ÍCONES DO MENU --- */
#define SYM_MENU_START   "\xF0\x9F\x9A\x80 "  /* 🚀 */
#define SYM_MENU_ROBOT   "\xF0\x9F\xA4\x96 "  /* 🤖 */
#define SYM_MENU_SCORE   "\xE2\xAD\x90 "      /* ⭐ */
#define SYM_MENU_EXIT    "\xF0\x9F\x92\x80 "  /* 💀 */

/* --- COBRA / PERSONAGEM (ASTRONAUTA) --- */
#define SYM_HEAD         "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x9A\x80 "  /* 👨‍🚀 */
#define SYM_HEAD_AUTO    "\xF0\x9F\xA4\x96 "  /* 🤖 */
#define SYM_HEAD_SUPER   "\xF0\x9F\x91\xBD "  /* 👽 */
#define SYM_BODY         "\xE2\xAD\x90 "      /* ⭐ (estrelas formam o corpo) */
#define SYM_BODY_SUPER   "\xF0\x9F\x8C\x9F "  /* 🌟 */
#define SYM_TAIL         "\xE2\x9C\xA8 "      /* ✨ */

/* --- COLETÁVEIS (ESPACIAIS) --- */
#define SYM_APPLE        "\xF0\x9F\x94\xAE "  /* 🔮 Cristal energético */
#define SYM_GRAPE        "\xF0\x9F\x8C\x80 "  /* 🌀 Portal */
#define SYM_CHERRY       "\xF0\x9F\x94\x8B "  /* 🔋 Bateria */
#define SYM_STAR         "\xF0\x9F\x9B\xB8 "  /* 🛸 UFO */
#define SYM_GEM          "\xF0\x9F\x92\xA0 "  /* 💠 Gema espacial */
#define SYM_SUPERSTAR    "\xF0\x9F\x8C\x9F "  /* 🌟 Super estrela */

/* --- NOVOS COLETÁVEIS GALÁCTICOS --- */
#define SYM_NEBULA       "\xF0\x9F\x8C\x8C "  /* 🌌 Nebulosa */
#define SYM_OXYGEN       "\xF0\x9F\x92\xA8 "  /* 💨 Oxigênio */
#define SYM_FUEL         "\xE2\x9B\xBD "      /* ⛽ Combustível */
#define SYM_SATELLITE_ITEM "\xF0\x9F\x93\xA1 " /* 📡 Satélite */

/* --- OBSTÁCULOS (MAIS VISÍVEIS) --- */
#define SYM_OBSTACLE     "\xE2\x9A\xAB "      /* ⚫ Buraco negro */
#define SYM_ASTEROID     "\xE2\x98\x84\xEF\xB8\x8F "  /* ☄️ Asteroide */
#define SYM_METEOR_OBS   "\xF0\x9F\x8C\xA0 "  /* 🌠 Meteoro */
#define SYM_DEBRIS       "\xF0\x9F\x9B\xA1 "  /* 🛡️ Destroço espacial */
#define SYM_BLACKHOLE    "\xF0\x9F\x95\xB3 "  /* 🕳️ Buraco negro */
#define SYM_DANGER       "\xE2\x9A\xA0\xEF\xB8\x8F "  /* ⚠️ Perigo */
#define SYM_ROCK         "\xF0\x9F\xAA\xA8 "  /* 🪨 Rocha espacial */
#define SYM_EXPLOSION    "\xF0\x9F\x92\xA5 "  /* 💥 Explosão/mina */

/* --- UI & STATUS --- */
#define SYM_PAUSE        "\xE2\x8F\xB8 "      /* ⏸ */
#define SYM_SKULL        "\xF0\x9F\x92\x80 "  /* 💀 */
#define SYM_TROPHY       "\xF0\x9F\x8F\x86 "  /* 🏆 */
#define SYM_CROWN        "\xF0\x9F\x91\x91 "  /* 👑 */
#define SYM_CLOCK        "\xE2\x8C\x9B "      /* ⌛ */
#define SYM_CTRL         "\xF0\x9F\x8E\xAE "  /* 🎮 (mudei para controle) */
#define SYM_FIRE         "\xF0\x9F\x92\xA5 "  /* 💥 */
#define SYM_HEART        "\xF0\x9F\x92\x9F "  /* 💟 */
#define SYM_BRAIN        "\xF0\x9F\xA7\xA0 "  /* 🧠 */
#define SYM_PATH         "\xC2\xB7 "          /* · */

/* --- NOVOS ÍCONES DE STATUS --- */
#define SYM_SHIELD       "\xF0\x9F\x9B\xA1 "  /* 🛡️ Escudo ativo */
#define SYM_BOOST        "\xE2\x9A\xA1 "      /* ⚡ Boost */
#define SYM_LIFE         "\xE2\x9D\xA4\xEF\xB8\x8F "  /* ❤️ Vida */

/* --- DECORAÇÃO (GALÁXIA) --- */
#define SYM_STAR_BG      "\xE2\x9C\xA6 "      /* ✦ Estrela de fundo */
#define SYM_STAR_SMALL   "\xC2\xB7 "          /* · Estrela pequena */
#define SYM_STAR_MED     "\xE2\x80\xA2 "      /* • Estrela média */
#define SYM_METEOR       "\xE2\x98\x84 "      /* ☄ */
#define SYM_ROCKET       "\xF0\x9F\x9A\x80 "  /* 🚀 */
#define SYM_PLANET       "\xF0\x9F\xAA\x90 "  /* 🪐 Planeta */
#define SYM_GALAXY       "\xF0\x9F\x8C\x8C "  /* 🌌 Galáxia */
#define SYM_SATELLITE    "\xF0\x9F\x9B\xB0 "  /* 🛰️ Satélite */
#define SYM_COMET        "\xE2\x98\x84 "      /* ☄ Cometa */
#define SYM_MOON         "\xF0\x9F\x8C\x99 "  /* 🌙 Lua */
#define SYM_SUN          "\xE2\x98\x80\xEF\xB8\x8F "  /* ☀️ Sol */
#define SYM_EARTH        "\xF0\x9F\x8C\x8D "  /* 🌍 Terra */
#define SYM_MARS         "\xF0\x9F\x94\xB4 "  /* 🔴 Marte */
#define SYM_ALIEN        "\xF0\x9F\x91\xBE "  /* 👾 Alien */

/* --- VELOCIDADE --- */
#define SYM_SPEED_SLOW   "\xF0\x9F\x90\x8C "  /* 🐌 */
#define SYM_SPEED_MED    "\xF0\x9F\x9B\xB0 "  /* 🛰️ */
#define SYM_SPEED_FAST   "\xF0\x9F\x9A\x80 "  /* 🚀 */
#define SYM_SPEED_INS    "\xE2\x9A\xA1 "      /* ⚡ (mudei para raio) */

/* --- BOX DRAWING (BORDAS - PADRÃO UNICODE) --- */
/* Borda Simples */
#define BOX_TL           "\xE2\x94\x8F"       /* ┏ */
#define BOX_TR           "\xE2\x94\x93"       /* ┓ */
#define BOX_BL           "\xE2\x94\x97"       /* ┗ */
#define BOX_BR           "\xE2\x94\x9B"       /* ┛ */
#define BOX_H            "\xE2\x94\x81"       /* ━ */
#define BOX_V            "\xE2\x94\x83"       /* ┃ */

/* Borda Dupla */
#define BOX_TLH          "\xE2\x95\x94"       /* ╔ */
#define BOX_TRH          "\xE2\x95\x97"       /* ╗ */
#define BOX_BLH          "\xE2\x95\x9A"       /* ╚ */
#define BOX_BRH          "\xE2\x95\x9D"       /* ╝ */
#define BOX_DH           "\xE2\x95\x90"       /* ═ */
#define BOX_DV           "\xE2\x95\x91"       /* ║ */

/* Conectores */
#define BOX_LM           "\xE2\x95\xA0"       /* ╠ */
#define BOX_RM           "\xE2\x95\xA3"       /* ╣ */

/* Blocos e Barras */
#define BLOCK            "\xE2\x96\x93"       /* ▓ */
#define BAR_FULL         "\xE2\x96\x88"       /* █ */
#define BAR_HALF         "\xE2\x96\x92"       /* ▒ */

/* --- EFEITOS VISUAIS --- */
#define SPARKLE          "\xE2\x9C\xA8 "      /* ✨ */
#define GLOW             "\xF0\x9F\x8C\x9F "  /* 🌟 */
#define TWINKLE          "\xE2\xAD\x90 "      /* ⭐ */

#endif