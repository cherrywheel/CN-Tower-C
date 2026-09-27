export type TestMethod = 'qemu' | 'real vm' | 'openwrt rootfs' | 'native' | 'only built' | 'wasi';

export interface TargetInfo {
  id: string;
  os: string;
  arch: string;
  filename: string;
  downloadUrl: string;
  testMethod: TestMethod;
  testDetailEn: string;
  testDetailRu: string;
  testDetailBe: string;
  deviceEn: string;
  deviceRu: string;
  deviceBe: string;
  category: 'windows' | 'macos' | 'linux' | 'bsd' | 'openwrt' | 'wasm';
}

const RELEASES_BASE = 'https://github.com/cherrywheel/CN-Tower-C/releases/latest/download';

export const LINUX_ARCHS = [
  { arch: 'x86_64', descEn: 'your pc', descRu: 'твой пк', descBe: 'твой пк' },
  { arch: 'i686', descEn: 'your old pc', descRu: 'старый пк', descBe: 'стары пк' },
  { arch: 'aarch64', descEn: 'raspberry pi 4 and 5, arm servers, baikal-m', descRu: 'raspberry pi 4 и 5, arm-серверы, baikal-m', descBe: 'raspberry pi 4 і 5, arm-серверы, baikal-m' },
  { arch: 'armhf', descEn: 'raspberry pi 2 and 3 on 32 bit', descRu: 'raspberry pi 2 и 3 в 32 бита', descBe: 'raspberry pi 2 і 3 у 32 біты' },
  { arch: 'armel', descEn: 'raspberry pi 1 and zero, old routers', descRu: 'raspberry pi 1 и zero, старые роутеры', descBe: 'raspberry pi 1 і zero, старыя роўтары' },
  { arch: 'riscv64', descEn: 'risc-v boards', descRu: 'платы risc-v', descBe: 'платы risc-v' },
  { arch: 'loongarch64', descEn: 'loongson', descRu: 'процессоры loongson', descBe: 'працэсары loongson' },
  { arch: 'mipsel', descEn: 'baikal-t1 and routers', descRu: 'baikal-t1 и роутеры', descBe: 'baikal-t1 і роўтары' },
  { arch: 'mips', descEn: 'big endian routers', descRu: 'big-endian роутеры', descBe: 'big-endian роўтары' },
  { arch: 'mips64el', descEn: '64 bit mips (little endian)', descRu: '64-битный mips (little-endian)', descBe: '64-бітны mips (little-endian)' },
  { arch: 'mips64', descEn: '64 bit mips (big endian)', descRu: '64-битный mips (big-endian)', descBe: '64-бітны mips (big-endian)' },
  { arch: 'ppc64le', descEn: 'ibm power', descRu: 'серверы ibm power', descBe: 'серверы ibm power' },
  { arch: 'ppc64', descEn: '64 bit big endian power', descRu: '64-битный big-endian power', descBe: '64-бітны big-endian power' },
  { arch: 'powerpc', descEn: 'old power macs and 32 bit power', descRu: 'старые power mac и 32-битный power', descBe: 'старыя power mac і 32-бітны power' },
  { arch: 's390x', descEn: 'ibm mainframes', descRu: 'мейнфреймы ibm', descBe: 'мейнфрэймы ibm' },
  { arch: 'sparc64', descEn: 'sun and oracle sparc', descRu: 'рабочие станции sun и oracle sparc', descBe: 'працоўныя станцыі sun і oracle sparc' },
  { arch: 'e2k', descEn: 'elbrus 2c3, 12c and 16c', descRu: 'эльбрус 2с3, 12с и 16с', descBe: 'эльбрус 2с3, 12с і 16с' },
  { arch: 'alpha', descEn: 'museum pieces', descRu: 'музейные экспонаты', descBe: 'музейныя экспанаты' },
  { arch: 'hppa', descEn: 'museum pieces (hp pa-risc)', descRu: 'музейные экспонаты (hp pa-risc)', descBe: 'музейныя экспанаты (hp pa-risc)' },
  { arch: 'm68k', descEn: 'museum pieces (motorola 68k)', descRu: 'музейные экспонаты (motorola 68k)', descBe: 'музейныя экспанаты (motorola 68k)' },
  { arch: 'sh4', descEn: 'museum pieces (superh)', descRu: 'музейные экспонаты (hitachi sh4)', descBe: 'музейныя экспанаты (hitachi sh4)' },
];

export const OPENWRT_ARCHS = [
  { arch: 'mips_24kc', descEn: 'ath79, like most old tp-links', descRu: 'ath79, большинство старых tp-link', descBe: 'ath79, большасць старых tp-link', tested: true },
  { arch: 'mipsel_24kc', descEn: 'ramips, like mt7621 ones', descRu: 'ramips, например mt7621', descBe: 'ramips, напрыклад mt7621', tested: false },
  { arch: 'aarch64_cortex-a53', descEn: 'mediatek filogic, mt7622', descRu: 'mediatek filogic, mt7622', descBe: 'mediatek filogic, mt7622', tested: false },
  { arch: 'aarch64_generic', descEn: 'arm64 boxes', descRu: 'arm64-коробки', descBe: 'arm64-скрынкі', tested: true },
  { arch: 'arm_cortex-a7_neon-vfpv4', descEn: 'ipq40xx', descRu: 'чипы ipq40xx', descBe: 'чыпы ipq40xx', tested: false },
  { arch: 'arm_cortex-a9_vfpv3-d16', descEn: 'mvebu, like wrt routers', descRu: 'mvebu, роутеры серии wrt', descBe: 'mvebu, роўтары серыі wrt', tested: true },
  { arch: 'x86_64', descEn: 'pc and vm installs', descRu: 'pc и виртуалки', descBe: 'pc і віртуалкі', tested: true },
];

export const ALL_TARGETS: TargetInfo[] = [
  // Windows
  {
    id: 'win-x64',
    os: 'Windows',
    arch: 'x64',
    filename: 'cn_tower_game-windows-x64.exe',
    downloadUrl: `${RELEASES_BASE}/cn_tower_game-windows-x64.exe`,
    testMethod: 'native',
    testDetailEn: 'runs directly on windows runner to the win',
    testDetailRu: 'проходится до победы на windows-раннере',
    testDetailBe: 'праходзіцца да перамогі на windows-ранеры',
    deviceEn: 'modern 64-bit windows pcs',
    deviceRu: 'современные 64-битные пк на windows',
    deviceBe: 'сучасныя 64-бітныя пк на windows',
    category: 'windows',
  },
  {
    id: 'win-x86',
    os: 'Windows',
    arch: 'x86',
    filename: 'cn_tower_game-windows-x86.exe',
    downloadUrl: `${RELEASES_BASE}/cn_tower_game-windows-x86.exe`,
    testMethod: 'native',
    testDetailEn: 'built and run on windows runner to the win',
    testDetailRu: 'собирается и проходится до победы',
    testDetailBe: 'збіраецца і праходзіцца да перамогі',
    deviceEn: '32-bit legacy windows installations',
    deviceRu: '32-битные системы windows',
    deviceBe: '32-бітныя сістэмы windows',
    category: 'windows',
  },
  {
    id: 'win-arm64',
    os: 'Windows',
    arch: 'arm64',
    filename: 'cn_tower_game-windows-arm64.exe',
    downloadUrl: `${RELEASES_BASE}/cn_tower_game-windows-arm64.exe`,
    testMethod: 'only built',
    testDetailEn: 'only built, ci does not run it (idgaf)',
    testDetailRu: 'только собирается, ci не запускает (idgaf)',
    testDetailBe: 'толькі збіраецца, ci не запускае (idgaf)',
    deviceEn: 'qualcomm snapdragon laptops',
    deviceRu: 'ноутбуки на snapdragon',
    deviceBe: 'наўтбукі на snapdragon',
    category: 'windows',
  },

  // macOS
  {
    id: 'mac-universal',
    os: 'macOS',
    arch: 'universal',
    filename: 'cn_tower_game-macos-universal',
    downloadUrl: `${RELEASES_BASE}/cn_tower_game-macos-universal`,
    testMethod: 'native',
    testDetailEn: 'native macos runner, passes to the win',
    testDetailRu: 'родной macos раннер, проходится до победы',
    testDetailBe: 'родны macos ранер, праходзіцца да перамогі',
    deviceEn: 'apple silicon (m1-m4) and intel, macos 11+',
    deviceRu: 'apple silicon (m1-m4) и intel, macos 11+',
    deviceBe: 'apple silicon (m1-m4) і intel, macos 11+',
    category: 'macos',
  },

  // Linux (21)
  ...LINUX_ARCHS.map((item) => {
    const isE2k = item.arch === 'e2k';
    return {
      id: `linux-${item.arch}`,
      os: 'Linux',
      arch: item.arch,
      filename: `cn_tower_game-linux-${item.arch}`,
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-linux-${item.arch}`,
      testMethod: 'qemu' as TestMethod,
      testDetailEn: isE2k
        ? 'mcst lcc 1.31.05 compiler + qemu-e2k to the win'
        : 'cross-compiled & played through to the win under qemu',
      testDetailRu: isE2k
        ? 'компилятор мцст lcc 1.31.05 + qemu-e2k до победы'
        : 'кросс-компиляция и прогон под qemu до победы',
      testDetailBe: isE2k
        ? 'кампілятар мцст lcc 1.31.05 + qemu-e2k да перамогі'
        : 'крос-кампіляцыя і прагон пад qemu да перамогі',
      deviceEn: item.descEn,
      deviceRu: item.descRu,
      deviceBe: item.descBe,
      category: 'linux' as const,
    };
  }),

  // BSD & Friends (7)
  ...['freebsd', 'openbsd', 'netbsd', 'dragonflybsd', 'solaris', 'illumos', 'haiku'].map((osName) => ({
    id: `bsd-${osName}`,
    os: osName === 'haiku' ? 'Haiku' : osName.toUpperCase(),
    arch: 'x86_64',
    filename: `cn_tower_game-${osName}-x86_64`,
    downloadUrl: `${RELEASES_BASE}/cn_tower_game-${osName}-x86_64`,
    testMethod: 'real vm' as TestMethod,
    testDetailEn: 'tested inside a dedicated real vm to the win',
    testDetailRu: 'проверяется в настоящей виртуалке до победы',
    testDetailBe: 'правяраецца ў сапраўднай віртуалцы да перамогі',
    deviceEn: `${osName} x86_64 machines and vms`,
    deviceRu: `системы и виртуалки ${osName} x86_64`,
    deviceBe: `сістэмы і віртуалкі ${osName} x86_64`,
    category: 'bsd' as const,
  })),

  // OpenWrt 24.10 (7)
  ...OPENWRT_ARCHS.map((item) => ({
    id: `owrt-24-${item.arch}`,
    os: 'OpenWrt 24.10 (ipk)',
    arch: item.arch,
    filename: `cn-tower-openwrt-24.10-${item.arch}.ipk`,
    downloadUrl: `${RELEASES_BASE}/cn-tower-openwrt-24.10-${item.arch}.ipk`,
    testMethod: (item.tested ? 'openwrt rootfs' : 'only built') as TestMethod,
    testDetailEn: item.tested
      ? 'installed with opkg in official rootfs & played to win'
      : 'built with official sdk, no rootfs image in ci',
    testDetailRu: item.tested
      ? 'ставится через opkg в настоящий rootfs и проходится'
      : 'собран официальным sdk, в ci нет rootfs-образа',
    testDetailBe: item.tested
      ? 'ставіцца праз opkg у сапраўдны rootfs і праходзіцца'
      : 'сабраны афіцыйным sdk, у ci няма rootfs-вобраза',
    deviceEn: item.descEn,
    deviceRu: item.descRu,
    deviceBe: item.descBe,
    category: 'openwrt' as const,
  })),

  // OpenWrt 25.12 (7)
  ...OPENWRT_ARCHS.map((item) => ({
    id: `owrt-25-${item.arch}`,
    os: 'OpenWrt 25.12 (apk)',
    arch: item.arch,
    filename: `cn-tower-openwrt-25.12-${item.arch}.apk`,
    downloadUrl: `${RELEASES_BASE}/cn-tower-openwrt-25.12-${item.arch}.apk`,
    testMethod: (item.tested ? 'openwrt rootfs' : 'only built') as TestMethod,
    testDetailEn: item.tested
      ? 'installed with apk in official rootfs & played to win'
      : 'built with official sdk, no rootfs image in ci',
    testDetailRu: item.tested
      ? 'ставится через apk в настоящий rootfs и проходится'
      : 'собран официальным sdk, в ci нет rootfs-образа',
    testDetailBe: item.tested
      ? 'ставіцца праз apk у сапраўдны rootfs і праходзіцца'
      : 'сабраны афіцыйным sdk, у ci няма rootfs-вобраза',
    deviceEn: item.descEn,
    deviceRu: item.descRu,
    deviceBe: item.descBe,
    category: 'openwrt' as const,
  })),

  // WebAssembly (1)
  {
    id: 'wasm-wasi',
    os: 'WebAssembly',
    arch: 'wasm32-wasi',
    filename: 'cn_tower_game-wasm32-wasi.wasm',
    downloadUrl: `${RELEASES_BASE}/cn_tower_game-wasm32-wasi.wasm`,
    testMethod: 'wasi',
    testDetailEn: 'tested with wasmtime in plain mode to the win',
    testDetailRu: 'проверяется через wasmtime в обычном режиме',
    testDetailBe: 'правяраецца праз wasmtime у звычайным рэжыме',
    deviceEn: 'any machine with wasmtime or a wasi runtime',
    deviceRu: 'любая система с wasmtime или wasi runtime',
    deviceBe: 'любая сістэма з wasmtime або wasi runtime',
    category: 'wasm',
  },
];
