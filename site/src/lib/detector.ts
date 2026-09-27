export interface DetectedPlatform {
  osName: string;
  arch: string;
  filename: string;
  downloadUrl: string;
  runCommand: string;
  isArm64Windows?: boolean;
}

const RELEASES_BASE = 'https://github.com/cherrywheel/CN-Tower-C/releases/latest/download';

export function detectUserPlatform(): DetectedPlatform {
  if (typeof window === 'undefined') {
    return {
      osName: 'linux',
      arch: 'x86_64',
      filename: 'cn_tower_game-linux-x86_64',
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-linux-x86_64`,
      runCommand: 'chmod +x cn_tower_game-linux-x86_64 && ./cn_tower_game-linux-x86_64',
    };
  }

  const ua = window.navigator.userAgent.toLowerCase();
  const platform = (window.navigator.platform || '').toLowerCase();

  // macOS
  if (platform.includes('mac') || ua.includes('mac os') || ua.includes('macintosh')) {
    return {
      osName: 'macos',
      arch: 'universal (apple silicon + intel)',
      filename: 'cn_tower_game-macos-universal',
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-macos-universal`,
      runCommand: 'chmod +x cn_tower_game-macos-universal && xattr -d com.apple.quarantine cn_tower_game-macos-universal && ./cn_tower_game-macos-universal',
    };
  }

  // Windows
  if (platform.includes('win') || ua.includes('windows')) {
    const isArm = ua.includes('arm64') || ua.includes('arm');
    const is64 = ua.includes('win64') || ua.includes('x64') || ua.includes('wow64');

    if (isArm) {
      return {
        osName: 'windows',
        arch: 'arm64',
        filename: 'cn_tower_game-windows-arm64.exe',
        downloadUrl: `${RELEASES_BASE}/cn_tower_game-windows-arm64.exe`,
        runCommand: '.\\cn_tower_game-windows-arm64.exe',
        isArm64Windows: true,
      };
    }

    if (is64) {
      return {
        osName: 'windows',
        arch: 'x64',
        filename: 'cn_tower_game-windows-x64.exe',
        downloadUrl: `${RELEASES_BASE}/cn_tower_game-windows-x64.exe`,
        runCommand: '.\\cn_tower_game-windows-x64.exe',
      };
    }

    return {
      osName: 'windows',
      arch: 'x86',
      filename: 'cn_tower_game-windows-x86.exe',
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-windows-x86.exe`,
      runCommand: '.\\cn_tower_game-windows-x86.exe',
    };
  }

  // FreeBSD / OpenBSD / NetBSD / Dragonfly / Solaris / Haiku
  if (ua.includes('freebsd')) {
    return {
      osName: 'freebsd',
      arch: 'x86_64',
      filename: 'cn_tower_game-freebsd-x86_64',
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-freebsd-x86_64`,
      runCommand: 'chmod +x cn_tower_game-freebsd-x86_64 && ./cn_tower_game-freebsd-x86_64',
    };
  }
  if (ua.includes('openbsd')) {
    return {
      osName: 'openbsd',
      arch: 'x86_64',
      filename: 'cn_tower_game-openbsd-x86_64',
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-openbsd-x86_64`,
      runCommand: 'chmod +x cn_tower_game-openbsd-x86_64 && ./cn_tower_game-openbsd-x86_64',
    };
  }
  if (ua.includes('haiku')) {
    return {
      osName: 'haiku',
      arch: 'x86_64',
      filename: 'cn_tower_game-haiku-x86_64',
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-haiku-x86_64`,
      runCommand: 'chmod +x cn_tower_game-haiku-x86_64 && ./cn_tower_game-haiku-x86_64',
    };
  }

  // Linux
  const isAarch64 = ua.includes('aarch64') || ua.includes('armv8') || ua.includes('arm64');
  const isArm32 = ua.includes('armv7') || ua.includes('arm');

  if (isAarch64) {
    return {
      osName: 'linux',
      arch: 'aarch64',
      filename: 'cn_tower_game-linux-aarch64',
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-linux-aarch64`,
      runCommand: 'chmod +x cn_tower_game-linux-aarch64 && ./cn_tower_game-linux-aarch64',
    };
  }

  if (isArm32) {
    return {
      osName: 'linux',
      arch: 'armhf',
      filename: 'cn_tower_game-linux-armhf',
      downloadUrl: `${RELEASES_BASE}/cn_tower_game-linux-armhf`,
      runCommand: 'chmod +x cn_tower_game-linux-armhf && ./cn_tower_game-linux-armhf',
    };
  }

  // Default Linux x86_64
  return {
    osName: 'linux',
    arch: 'x86_64',
    filename: 'cn_tower_game-linux-x86_64',
    downloadUrl: `${RELEASES_BASE}/cn_tower_game-linux-x86_64`,
    runCommand: 'chmod +x cn_tower_game-linux-x86_64 && ./cn_tower_game-linux-x86_64',
  };
}
