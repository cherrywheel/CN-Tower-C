import React, { useState } from 'react';
import type { Language } from '../i18n';
import { T } from '../i18n';
import { Terminal, Copy, Check, Package } from 'lucide-react';

interface BuildSectionProps {
  lang: Language;
}

export const BuildSection: React.FC<BuildSectionProps> = ({ lang }) => {
  const t = T[lang];
  const [activeTab, setActiveTab] = useState<'unix' | 'windows' | 'cross' | 'manual'>('unix');
  const [copiedId, setCopiedId] = useState<string | null>(null);

  const copyText = (text: string, id: string) => {
    navigator.clipboard.writeText(text);
    setCopiedId(id);
    setTimeout(() => setCopiedId(null), 2000);
  };

  const SNIPPETS = {
    unix: `# any unix-like: linux, macos, bsd, solaris, haiku, termux, wsl, msys2
sh setup.sh

# or skip all interactive prompts:
sh setup.sh --yes`,

    windows: `:: windows developer command prompt
setup.cmd`,

    cross: `# debian / ubuntu: build and test all 20 linux architectures under qemu
pip install ziglang  # needed for loongarch64
sh setup.sh --cross --out dist/`,

    manual: `# linux / macos / bsd
cd src
make

# windows (msvc developer command prompt)
cd src
nmake

# elbrus with mcst lcc
cd src
make CC=lcc`,
  };

  const PKG_MANAGERS = [
    'apt', 'dnf', 'yum', 'pacman', 'zypper', 'apk', 'xbps', 'emerge',
    'eopkg', 'swupd', 'nix', 'brew', 'pkg', 'pkg_add', 'pkgin', 'pkgman', 'winget'
  ];

  return (
    <section id="build" className="border-t border-border py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <div>
          <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
            <span>04</span>
            <span className="text-muted-foreground">//</span>
            <span>{t.build.heading}</span>
          </div>
          <h2 className="mt-2 font-mono text-3xl font-bold tracking-tight sm:text-4xl text-foreground">
            {t.build.heading}
          </h2>
          <p className="mt-2 max-w-2xl text-sm sm:text-base text-muted-foreground leading-relaxed">
            {t.build.sub}
          </p>
        </div>

        {/* Build Mode Tabs */}
        <div className="mt-8 flex flex-wrap gap-2">
          {[
            { id: 'unix', label: t.build.unixTitle },
            { id: 'windows', label: t.build.winTitle },
            { id: 'cross', label: t.build.crossTitle },
            { id: 'manual', label: t.build.manualTitle },
          ].map((tab) => (
            <button
              key={tab.id}
              onClick={() => setActiveTab(tab.id as typeof activeTab)}
              className={`rounded-lg border px-3.5 py-1.5 font-mono text-xs transition-all cursor-pointer ${
                activeTab === tab.id
                  ? 'border-accent bg-accent/15 text-foreground font-semibold shadow-xs'
                  : 'border-border bg-card text-muted-foreground hover:border-foreground/30 hover:text-foreground'
              }`}
            >
              {tab.label}
            </button>
          ))}
        </div>

        {/* Active Code Block */}
        <div className="mt-4 rounded-xl border border-border bg-card shadow-xs overflow-hidden">
          <div className="flex items-center justify-between border-b border-border bg-muted/40 px-4 py-2.5">
            <div className="flex items-center gap-2 font-mono text-xs text-muted-foreground">
              <Terminal className="h-3.5 w-3.5 text-accent" />
              <span>
                {activeTab === 'unix' && 'setup.sh'}
                {activeTab === 'windows' && 'setup.cmd'}
                {activeTab === 'cross' && 'setup.sh --cross'}
                {activeTab === 'manual' && 'make / nmake'}
              </span>
            </div>
            <button
              onClick={() => copyText(SNIPPETS[activeTab], activeTab)}
              className="inline-flex items-center gap-1.5 font-mono text-xs text-muted-foreground hover:text-foreground transition-colors cursor-pointer"
            >
              {copiedId === activeTab ? (
                <>
                  <Check className="h-3.5 w-3.5 text-emerald-400" />
                  <span className="text-emerald-400">{t.download.copied}</span>
                </>
              ) : (
                <>
                  <Copy className="h-3.5 w-3.5" />
                  <span>{t.download.copy}</span>
                </>
              )}
            </button>
          </div>

          <pre className="p-4 font-mono text-xs sm:text-sm text-foreground overflow-x-auto leading-relaxed">
            <code>{SNIPPETS[activeTab]}</code>
          </pre>

          <div className="border-t border-border bg-muted/20 px-4 py-3 text-xs text-muted-foreground font-sans">
            {activeTab === 'unix' && t.build.unixDesc}
            {activeTab === 'windows' && t.build.winDesc}
            {activeTab === 'cross' && t.build.crossDesc}
            {activeTab === 'manual' && t.build.manualDesc}
          </div>
        </div>

        {/* Known package managers list */}
        <div className="mt-8 rounded-xl border border-border bg-card p-5">
          <div className="flex items-center gap-2 font-mono text-xs font-semibold text-foreground uppercase tracking-wider mb-3">
            <Package className="h-4 w-4 text-accent" />
            <span>{t.build.pkgManagersTitle}</span>
          </div>
          <p className="text-xs text-muted-foreground mb-3 font-sans">
            {lang === 'be'
              ? 'setup.sh сам вызначае дыстрыбутыў і прапануе паставіць кампілятар:'
              : lang === 'ru'
              ? 'setup.sh сам определяет дистрибутив и предлагает поставить компилятор:'
              : 'setup.sh automatically detects your distro and offers to install a c compiler:'}
          </p>
          <div className="flex flex-wrap gap-1.5 font-mono text-[11px]">
            {PKG_MANAGERS.map((pkg) => (
              <span
                key={pkg}
                className="rounded bg-muted px-2 py-0.5 text-foreground/80 border border-border/60"
              >
                {pkg}
              </span>
            ))}
          </div>
        </div>
      </div>
    </section>
  );
};
