import React, { useState } from 'react';
import SpotlightCard from './SpotlightCard';
import type { Language } from '../i18n';
import { T } from '../i18n';
import { detectUserPlatform } from '../lib/detector';
import { LINUX_ARCHS, OPENWRT_ARCHS } from '../data/platforms';
import {
  Download,
  Copy,
  Check,
  ExternalLink,
  Laptop,
  Apple,
  Search,
} from 'lucide-react';

interface DownloadSectionProps {
  lang: Language;
}

export const DownloadSection: React.FC<DownloadSectionProps> = ({ lang }) => {
  const t = T[lang];
  const detected = detectUserPlatform();

  const [copiedId, setCopiedId] = useState<string | null>(null);
  const [activeGroup, setActiveGroup] = useState<'windows' | 'macos' | 'linux' | 'bsd' | 'openwrt' | 'wasm'>('linux');
  const [linuxSearch, setLinuxSearch] = useState('');

  const RELEASES_LATEST = 'https://github.com/cherrywheel/CN-Tower-C/releases/latest';
  const RELEASES_BASE = 'https://github.com/cherrywheel/CN-Tower-C/releases/latest/download';

  const copyText = (text: string, id: string) => {
    navigator.clipboard.writeText(text);
    setCopiedId(id);
    setTimeout(() => setCopiedId(null), 2000);
  };

  const getLinuxDesc = (item: (typeof LINUX_ARCHS)[number]) => {
    if (lang === 'be' && item.descBe) return item.descBe;
    if (lang === 'ru') return item.descRu;
    return item.descEn;
  };

  const getOpenWrtDesc = (item: (typeof OPENWRT_ARCHS)[number]) => {
    if (lang === 'be' && item.descBe) return item.descBe;
    if (lang === 'ru') return item.descRu;
    return item.descEn;
  };

  const filteredLinux = LINUX_ARCHS.filter((item) => {
    const q = linuxSearch.toLowerCase().trim();
    if (!q) return true;
    const desc = getLinuxDesc(item);
    return item.arch.toLowerCase().includes(q) || desc.toLowerCase().includes(q);
  });

  return (
    <section id="download" className="border-t border-border py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        {/* Section title */}
        <div className="text-center sm:text-left">
          <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
            <span>01</span>
            <span className="text-muted-foreground">//</span>
            <span>{t.download.heading}</span>
          </div>
          <h2 className="mt-2 font-mono text-3xl font-bold tracking-tight sm:text-4xl text-foreground">
            {t.download.heading}
          </h2>
          <p className="mt-2 max-w-2xl text-sm sm:text-base text-muted-foreground leading-relaxed">
            {t.download.sub}
          </p>
        </div>

        {/* Highlighted Detected System Card */}
        <div className="mt-8">
          <SpotlightCard className="border-accent/40 bg-card/90 shadow-lg">
            <div className="flex flex-col gap-6 lg:flex-row lg:items-center lg:justify-between">
              <div className="space-y-2">
                <div className="inline-flex items-center gap-2 rounded-full border border-accent/30 bg-accent/10 px-3 py-0.5 text-xs font-mono text-accent">
                  <span className="h-1.5 w-1.5 rounded-full bg-accent" />
                  <span>{t.download.detectedBadge}</span>
                </div>
                <div className="flex flex-wrap items-baseline gap-2 font-mono">
                  <span className="text-2xl font-bold text-foreground capitalize">{detected.osName}</span>
                  <span className="text-sm text-muted-foreground">({detected.arch})</span>
                </div>
                <div className="font-mono text-xs text-muted-foreground break-all">
                  <span>file: </span>
                  <span className="text-foreground select-all">{detected.filename}</span>
                </div>
              </div>

              <div className="flex flex-col sm:flex-row items-stretch sm:items-center gap-3">
                <a
                  href={detected.downloadUrl}
                  className="inline-flex h-11 items-center justify-center gap-2 rounded-lg bg-accent px-6 font-mono text-sm font-semibold text-accent-foreground shadow-md transition-all hover:opacity-90 active:scale-98"
                >
                  <Download className="h-4 w-4" />
                  <span>
                    {t.download.quickDownload} ({detected.arch.split(' ')[0]})
                  </span>
                </a>
              </div>
            </div>

            {/* Quick run snippet */}
            <div className="mt-6 border-t border-border/80 pt-4">
              <div className="flex items-center justify-between font-mono text-xs text-muted-foreground mb-1.5">
                <span>{t.download.quickRun}</span>
                <button
                  onClick={() => copyText(detected.runCommand, 'detected-cmd')}
                  className="inline-flex items-center gap-1 text-xs hover:text-foreground transition-colors cursor-pointer"
                >
                  {copiedId === 'detected-cmd' ? (
                    <>
                      <Check className="h-3 w-3 text-emerald-400" />
                      <span className="text-emerald-400">{t.download.copied}</span>
                    </>
                  ) : (
                    <>
                      <Copy className="h-3 w-3" />
                      <span>{t.download.copy}</span>
                    </>
                  )}
                </button>
              </div>
              <div className="overflow-x-auto rounded-md bg-muted/60 p-3 font-mono text-xs text-foreground/90 border border-border/60">
                <code>{detected.runCommand}</code>
              </div>
              {detected.isArm64Windows && (
                <p className="mt-2 text-xs font-mono text-amber-500/90 leading-relaxed">
                  * {t.download.windowsArmNote}
                </p>
              )}
            </div>
          </SpotlightCard>
        </div>

        {/* Group Selector */}
        <div className="mt-14">
          <div className="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-4 border-b border-border pb-4">
            <h3 className="font-mono text-lg font-semibold text-foreground">
              {t.download.allPlatforms}
            </h3>
            <a
              href={RELEASES_LATEST}
              target="_blank"
              rel="noopener noreferrer"
              className="inline-flex items-center gap-1 font-mono text-xs text-accent hover:underline"
            >
              <span>github releases/latest</span>
              <ExternalLink className="h-3 w-3" />
            </a>
          </div>

          {/* Group tabs */}
          <div className="mt-4 flex flex-wrap gap-2">
            {[
              { id: 'windows', label: 'windows' },
              { id: 'macos', label: 'macos' },
              { id: 'linux', label: 'linux (20 archs)' },
              { id: 'bsd', label: 'bsd & friends' },
              { id: 'openwrt', label: 'openwrt' },
              { id: 'wasm', label: 'webassembly' },
            ].map((tab) => (
              <button
                key={tab.id}
                onClick={() => setActiveGroup(tab.id as typeof activeGroup)}
                className={`rounded-lg border px-3 py-1.5 font-mono text-xs capitalize transition-all cursor-pointer ${
                  activeGroup === tab.id
                    ? 'border-accent bg-accent/10 text-foreground font-semibold shadow-xs'
                    : 'border-border bg-card text-muted-foreground hover:border-foreground/30 hover:text-foreground'
                }`}
              >
                {tab.label}
              </button>
            ))}
          </div>

          {/* Windows group */}
          {activeGroup === 'windows' && (
            <div className="mt-6 space-y-4">
              <p className="font-mono text-xs text-muted-foreground leading-relaxed">
                {t.download.windowsArmNote}
              </p>
              <div className="grid gap-3 sm:grid-cols-3">
                {[
                  {
                    arch: 'x64',
                    file: 'cn_tower_game-windows-x64.exe',
                    desc: lang === 'be' ? 'сучасныя 64-бітныя пк' : lang === 'ru' ? 'современные 64-битные пк' : '64-bit windows pcs',
                  },
                  {
                    arch: 'x86',
                    file: 'cn_tower_game-windows-x86.exe',
                    desc: lang === 'be' ? 'старыя 32-бітныя сістэмы' : lang === 'ru' ? 'старые 32-битные системы' : '32-bit legacy windows',
                  },
                  {
                    arch: 'arm64',
                    file: 'cn_tower_game-windows-arm64.exe',
                    desc: lang === 'be' ? 'windows on arm (толькі зборка)' : lang === 'ru' ? 'windows on arm (только сборка)' : 'windows on arm (only built)',
                  },
                ].map((item) => (
                  <div key={item.arch} className="rounded-xl border border-border bg-card p-4 flex flex-col justify-between">
                    <div>
                      <div className="flex items-center justify-between">
                        <span className="font-mono text-base font-bold text-foreground uppercase">{item.arch}</span>
                        <Laptop className="h-4 w-4 text-muted-foreground" />
                      </div>
                      <p className="mt-1 text-xs text-muted-foreground font-sans">{item.desc}</p>
                      <div className="mt-3 font-mono text-[11px] text-muted-foreground/80 break-all select-all">
                        {item.file}
                      </div>
                    </div>
                    <a
                      href={`${RELEASES_BASE}/${item.file}`}
                      className="mt-4 inline-flex h-9 items-center justify-center gap-1.5 rounded-md bg-muted px-3 font-mono text-xs font-medium text-foreground hover:bg-accent hover:text-accent-foreground transition-colors"
                    >
                      <Download className="h-3.5 w-3.5" />
                      <span>{t.download.quickDownload}</span>
                    </a>
                  </div>
                ))}
              </div>
            </div>
          )}

          {/* macOS group */}
          {activeGroup === 'macos' && (
            <div className="mt-6 space-y-4">
              <div className="rounded-xl border border-border bg-card p-6">
                <div className="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-4">
                  <div>
                    <div className="inline-flex items-center gap-2 font-mono text-lg font-bold text-foreground">
                      <Apple className="h-5 w-5" />
                      <span>universal (apple silicon + intel)</span>
                    </div>
                    <p className="mt-1 text-xs text-muted-foreground">macOS 11+</p>
                    <div className="mt-2 font-mono text-xs text-muted-foreground select-all">
                      cn_tower_game-macos-universal
                    </div>
                  </div>
                  <a
                    href={`${RELEASES_BASE}/cn_tower_game-macos-universal`}
                    className="inline-flex h-10 items-center justify-center gap-2 rounded-lg bg-accent px-5 font-mono text-xs font-semibold text-accent-foreground shadow-sm transition-opacity hover:opacity-90"
                  >
                    <Download className="h-4 w-4" />
                    <span>{t.download.quickDownload} universal</span>
                  </a>
                </div>

                <div className="mt-6 border-t border-border/80 pt-4">
                  <p className="font-mono text-xs text-muted-foreground mb-2">
                    {t.download.macosNote}
                  </p>
                  <div className="flex items-center justify-between rounded-md bg-muted/60 p-3 font-mono text-xs border border-border/60">
                    <code className="text-foreground break-all">
                      chmod +x cn_tower_game-macos-universal && xattr -d com.apple.quarantine cn_tower_game-macos-universal
                    </code>
                    <button
                      onClick={() =>
                        copyText(
                          'chmod +x cn_tower_game-macos-universal && xattr -d com.apple.quarantine cn_tower_game-macos-universal',
                          'mac-quarantine'
                        )
                      }
                      className="ml-3 shrink-0 text-muted-foreground hover:text-foreground cursor-pointer"
                    >
                      {copiedId === 'mac-quarantine' ? <Check className="h-3.5 w-3.5 text-emerald-400" /> : <Copy className="h-3.5 w-3.5" />}
                    </button>
                  </div>
                </div>
              </div>
            </div>
          )}

          {/* Linux group (Table with 20 archs) */}
          {activeGroup === 'linux' && (
            <div className="mt-6 space-y-4">
              <div className="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-3">
                <p className="font-mono text-xs text-muted-foreground">
                  {t.download.linuxNote}
                </p>
                <div className="relative w-full sm:w-64">
                  <Search className="pointer-events-none absolute left-2.5 top-1/2 -translate-y-1/2 h-3.5 w-3.5 text-muted-foreground" />
                  <input
                    type="text"
                    placeholder={
                      lang === 'be'
                        ? 'пошук архітэктуры...'
                        : lang === 'ru'
                        ? 'поиск архитектуры...'
                        : 'filter architecture...'
                    }
                    value={linuxSearch}
                    onChange={(e) => setLinuxSearch(e.target.value)}
                    className="w-full rounded-md border border-border bg-card pl-8 pr-3 py-1.5 font-mono text-xs text-foreground placeholder:text-muted-foreground/60 focus:border-accent focus:outline-hidden"
                  />
                </div>
              </div>

              <div className="overflow-x-auto rounded-xl border border-border bg-card">
                <table className="w-full text-left font-mono text-xs">
                  <thead className="border-b border-border bg-muted/50 text-muted-foreground">
                    <tr>
                      <th className="px-4 py-3 font-semibold">{t.download.linuxArchCol}</th>
                      <th className="px-4 py-3 font-semibold">{t.download.linuxTargetCol}</th>
                      <th className="px-4 py-3 font-semibold text-right">{t.download.linuxDownloadCol}</th>
                    </tr>
                  </thead>
                  <tbody className="divide-y divide-border/60">
                    {filteredLinux.map((item) => (
                      <tr key={item.arch} className="hover:bg-muted/30 transition-colors">
                        <td className="px-4 py-2.5 font-bold text-foreground whitespace-nowrap">
                          {item.arch}
                        </td>
                        <td className="px-4 py-2.5 text-muted-foreground font-sans">
                          {getLinuxDesc(item)}
                        </td>
                        <td className="px-4 py-2.5 text-right whitespace-nowrap">
                          <a
                            href={`${RELEASES_BASE}/cn_tower_game-linux-${item.arch}`}
                            className="inline-flex items-center gap-1 rounded bg-muted px-2.5 py-1 text-[11px] font-mono text-foreground hover:bg-accent hover:text-accent-foreground transition-colors"
                          >
                            <Download className="h-3 w-3" />
                            <span>cn_tower_game-linux-{item.arch}</span>
                          </a>
                        </td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              </div>

              <div className="rounded-lg bg-muted/40 p-3 border border-border/60 text-xs font-mono text-muted-foreground flex items-center justify-between">
                <span>{t.download.unixPermissions} <code>chmod +x cn_tower_game-linux-&lt;arch&gt;</code></span>
                <button
                  onClick={() => copyText('chmod +x cn_tower_game-linux-x86_64 && ./cn_tower_game-linux-x86_64', 'linux-chmod')}
                  className="inline-flex items-center gap-1 hover:text-foreground text-[11px] cursor-pointer"
                >
                  {copiedId === 'linux-chmod' ? <Check className="h-3 w-3 text-emerald-400" /> : <Copy className="h-3 w-3" />}
                  <span>{t.download.copy}</span>
                </button>
              </div>
            </div>
          )}

          {/* BSD & Friends group */}
          {activeGroup === 'bsd' && (
            <div className="mt-6 space-y-4">
              <p className="font-mono text-xs text-muted-foreground leading-relaxed">
                {t.download.bsdNote}
              </p>
              <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
                {[
                  { os: 'FreeBSD', file: 'cn_tower_game-freebsd-x86_64' },
                  { os: 'OpenBSD', file: 'cn_tower_game-openbsd-x86_64' },
                  { os: 'NetBSD', file: 'cn_tower_game-netbsd-x86_64' },
                  { os: 'DragonFly BSD', file: 'cn_tower_game-dragonflybsd-x86_64' },
                  { os: 'Solaris', file: 'cn_tower_game-solaris-x86_64' },
                  { os: 'Illumos', file: 'cn_tower_game-illumos-x86_64' },
                  { os: 'Haiku', file: 'cn_tower_game-haiku-x86_64' },
                ].map((item) => (
                  <div key={item.os} className="rounded-xl border border-border bg-card p-4 flex flex-col justify-between">
                    <div>
                      <div className="flex items-center justify-between">
                        <span className="font-mono text-sm font-bold text-foreground">{item.os}</span>
                        <span className="font-mono text-[11px] text-muted-foreground">x86_64</span>
                      </div>
                      <div className="mt-2 font-mono text-[11px] text-muted-foreground/80 break-all select-all">
                        {item.file}
                      </div>
                    </div>
                    <a
                      href={`${RELEASES_BASE}/${item.file}`}
                      className="mt-3 inline-flex h-8 items-center justify-center gap-1.5 rounded-md bg-muted px-3 font-mono text-xs text-foreground hover:bg-accent hover:text-accent-foreground transition-colors"
                    >
                      <Download className="h-3 w-3" />
                      <span>{t.download.quickDownload}</span>
                    </a>
                  </div>
                ))}
              </div>
            </div>
          )}

          {/* OpenWrt group */}
          {activeGroup === 'openwrt' && (
            <div className="mt-6 space-y-6">
              <p className="font-mono text-xs text-muted-foreground leading-relaxed">
                {t.download.openwrtNote}
              </p>

              {/* Arch check snippet */}
              <div className="rounded-lg border border-border bg-muted/40 p-4">
                <div className="flex items-center justify-between text-xs font-mono text-muted-foreground mb-1.5">
                  <span>{t.download.openwrtFindArch}</span>
                  <button
                    onClick={() => copyText('grep ARCH /etc/openwrt_release', 'grep-arch')}
                    className="inline-flex items-center gap-1 text-xs hover:text-foreground cursor-pointer"
                  >
                    {copiedId === 'grep-arch' ? <Check className="h-3 w-3 text-emerald-400" /> : <Copy className="h-3 w-3" />}
                    <span>{t.download.copy}</span>
                  </button>
                </div>
                <div className="rounded bg-background p-2.5 font-mono text-xs text-foreground border border-border/60">
                  <code>grep ARCH /etc/openwrt_release</code>
                </div>
              </div>

              {/* Packages Table */}
              <div className="overflow-x-auto rounded-xl border border-border bg-card">
                <table className="w-full text-left font-mono text-xs">
                  <thead className="border-b border-border bg-muted/50 text-muted-foreground">
                    <tr>
                      <th className="px-4 py-3 font-semibold">architecture</th>
                      <th className="px-4 py-3 font-semibold">chipsets / devices</th>
                      <th className="px-4 py-3 font-semibold text-center">24.10 (ipk)</th>
                      <th className="px-4 py-3 font-semibold text-center">25.12 (apk)</th>
                    </tr>
                  </thead>
                  <tbody className="divide-y divide-border/60">
                    {OPENWRT_ARCHS.map((item) => (
                      <tr key={item.arch} className="hover:bg-muted/30 transition-colors">
                        <td className="px-4 py-2.5 font-bold text-foreground whitespace-nowrap">
                          {item.arch}
                        </td>
                        <td className="px-4 py-2.5 text-muted-foreground font-sans">
                          {getOpenWrtDesc(item)}
                        </td>
                        <td className="px-4 py-2.5 text-center whitespace-nowrap">
                          <a
                            href={`${RELEASES_BASE}/cn-tower-openwrt-24.10-${item.arch}.ipk`}
                            className="inline-flex items-center gap-1 rounded bg-muted px-2 py-1 text-[11px] font-mono text-foreground hover:bg-accent hover:text-accent-foreground transition-colors"
                          >
                            <Download className="h-3 w-3" />
                            <span>.ipk</span>
                          </a>
                        </td>
                        <td className="px-4 py-2.5 text-center whitespace-nowrap">
                          <a
                            href={`${RELEASES_BASE}/cn-tower-openwrt-25.12-${item.arch}.apk`}
                            className="inline-flex items-center gap-1 rounded bg-muted px-2 py-1 text-[11px] font-mono text-foreground hover:bg-accent hover:text-accent-foreground transition-colors"
                          >
                            <Download className="h-3 w-3" />
                            <span>.apk</span>
                          </a>
                        </td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              </div>

              {/* Install instructions */}
              <div className="grid gap-4 sm:grid-cols-2">
                <div className="rounded-lg border border-border bg-card p-4">
                  <div className="flex items-center justify-between font-mono text-xs text-muted-foreground mb-1.5">
                    <span>{t.download.openwrtInstall24}</span>
                    <button
                      onClick={() =>
                        copyText(
                          'opkg install cn-tower-openwrt-24.10-mips_24kc.ipk\ncn-tower',
                          'owrt-opkg'
                        )
                      }
                      className="inline-flex items-center gap-1 text-[11px] hover:text-foreground cursor-pointer"
                    >
                      {copiedId === 'owrt-opkg' ? <Check className="h-3 w-3 text-emerald-400" /> : <Copy className="h-3 w-3" />}
                    </button>
                  </div>
                  <pre className="rounded bg-muted/60 p-2.5 font-mono text-xs text-foreground/90 overflow-x-auto">
                    opkg install cn-tower-openwrt-24.10-mips_24kc.ipk{'\n'}cn-tower
                  </pre>
                </div>

                <div className="rounded-lg border border-border bg-card p-4">
                  <div className="flex items-center justify-between font-mono text-xs text-muted-foreground mb-1.5">
                    <span>{t.download.openwrtInstall25}</span>
                    <button
                      onClick={() =>
                        copyText(
                          'apk add --allow-untrusted cn-tower-openwrt-25.12-mips_24kc.apk\ncn-tower',
                          'owrt-apk'
                        )
                      }
                      className="inline-flex items-center gap-1 text-[11px] hover:text-foreground cursor-pointer"
                    >
                      {copiedId === 'owrt-apk' ? <Check className="h-3 w-3 text-emerald-400" /> : <Copy className="h-3 w-3" />}
                    </button>
                  </div>
                  <pre className="rounded bg-muted/60 p-2.5 font-mono text-xs text-foreground/90 overflow-x-auto">
                    apk add --allow-untrusted cn-tower-openwrt-25.12-mips_24kc.apk{'\n'}cn-tower
                  </pre>
                </div>
              </div>
            </div>
          )}

          {/* WebAssembly group */}
          {activeGroup === 'wasm' && (
            <div className="mt-6 space-y-4">
              <div className="rounded-xl border border-border bg-card p-6">
                <div className="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-4">
                  <div>
                    <div className="font-mono text-lg font-bold text-foreground">
                      wasm32-wasi
                    </div>
                    <p className="mt-1 text-xs text-muted-foreground font-sans">
                      {t.download.wasmNote}
                    </p>
                    <div className="mt-2 font-mono text-xs text-muted-foreground select-all">
                      cn_tower_game-wasm32-wasi.wasm
                    </div>
                  </div>
                  <a
                    href={`${RELEASES_BASE}/cn_tower_game-wasm32-wasi.wasm`}
                    className="inline-flex h-10 items-center justify-center gap-2 rounded-lg bg-accent px-5 font-mono text-xs font-semibold text-accent-foreground shadow-sm transition-opacity hover:opacity-90"
                  >
                    <Download className="h-4 w-4" />
                    <span>{t.download.quickDownload} .wasm</span>
                  </a>
                </div>

                <div className="mt-6 grid gap-4 sm:grid-cols-2 border-t border-border/80 pt-4">
                  <div>
                    <div className="flex items-center justify-between text-xs font-mono text-muted-foreground mb-1">
                      <span>run with wasmtime:</span>
                      <button
                        onClick={() => copyText('wasmtime cn_tower_game-wasm32-wasi.wasm', 'wasm-run')}
                        className="text-muted-foreground hover:text-foreground cursor-pointer"
                      >
                        {copiedId === 'wasm-run' ? <Check className="h-3 w-3 text-emerald-400" /> : <Copy className="h-3 w-3" />}
                      </button>
                    </div>
                    <pre className="rounded bg-muted/60 p-2.5 font-mono text-xs text-foreground overflow-x-auto">
                      wasmtime cn_tower_game-wasm32-wasi.wasm
                    </pre>
                  </div>

                  <div>
                    <div className="flex items-center justify-between text-xs font-mono text-muted-foreground mb-1">
                      <span>run with saves preserved:</span>
                      <button
                        onClick={() => copyText('wasmtime --dir=. cn_tower_game-wasm32-wasi.wasm', 'wasm-save')}
                        className="text-muted-foreground hover:text-foreground cursor-pointer"
                      >
                        {copiedId === 'wasm-save' ? <Check className="h-3 w-3 text-emerald-400" /> : <Copy className="h-3 w-3" />}
                      </button>
                    </div>
                    <pre className="rounded bg-muted/60 p-2.5 font-mono text-xs text-foreground overflow-x-auto">
                      wasmtime --dir=. cn_tower_game-wasm32-wasi.wasm
                    </pre>
                  </div>
                </div>
              </div>
            </div>
          )}
        </div>
      </div>
    </section>
  );
};
