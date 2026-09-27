import React, { useState } from 'react';
import SpotlightCard from './SpotlightCard';
import type { Language } from '../i18n';
import { T } from '../i18n';
import { ALL_TARGETS, type TestMethod } from '../data/platforms';
import { Download, CheckCircle2 } from 'lucide-react';

interface RunsEverywhereProps {
  lang: Language;
}

export const RunsEverywhereSection: React.FC<RunsEverywhereProps> = ({ lang }) => {
  const t = T[lang];
  const [filter, setFilter] = useState<'all' | 'linux' | 'bsd' | 'openwrt' | 'desktop' | 'wasm'>('all');

  const filteredTargets = ALL_TARGETS.filter((target) => {
    if (filter === 'all') return true;
    if (filter === 'desktop') return target.category === 'windows' || target.category === 'macos';
    if (filter === 'bsd') return target.category === 'bsd';
    return target.category === filter;
  });

  const getMethodBadge = (method: TestMethod) => {
    switch (method) {
      case 'qemu':
        return (
          <span className="inline-flex items-center gap-1 rounded bg-emerald-500/10 border border-emerald-500/30 px-2 py-0.5 text-[10px] font-mono text-emerald-400">
            <span className="h-1.5 w-1.5 rounded-full bg-emerald-500 animate-pulse" />
            qemu
          </span>
        );
      case 'real vm':
        return (
          <span className="inline-flex items-center gap-1 rounded bg-sky-500/10 border border-sky-500/30 px-2 py-0.5 text-[10px] font-mono text-sky-400">
            <span className="h-1.5 w-1.5 rounded-full bg-sky-500" />
            real vm
          </span>
        );
      case 'openwrt rootfs':
        return (
          <span className="inline-flex items-center gap-1 rounded bg-amber-500/10 border border-amber-500/30 px-2 py-0.5 text-[10px] font-mono text-amber-400">
            <span className="h-1.5 w-1.5 rounded-full bg-amber-500" />
            rootfs opkg/apk
          </span>
        );
      case 'native':
        return (
          <span className="inline-flex items-center gap-1 rounded bg-rose-500/10 border border-rose-500/30 px-2 py-0.5 text-[10px] font-mono text-rose-400">
            <span className="h-1.5 w-1.5 rounded-full bg-rose-500" />
            native runner
          </span>
        );
      case 'wasi':
        return (
          <span className="inline-flex items-center gap-1 rounded bg-purple-500/10 border border-purple-500/30 px-2 py-0.5 text-[10px] font-mono text-purple-400">
            <span className="h-1.5 w-1.5 rounded-full bg-purple-500" />
            wasi runtime
          </span>
        );
      case 'only built':
        return (
          <span className="inline-flex items-center gap-1 rounded bg-muted/60 border border-border px-2 py-0.5 text-[10px] font-mono text-muted-foreground">
            only built
          </span>
        );
    }
  };

  const getDeviceDesc = (target: (typeof ALL_TARGETS)[number]) => {
    if (lang === 'be' && target.deviceBe) return target.deviceBe;
    if (lang === 'ru') return target.deviceRu;
    return target.deviceEn;
  };

  const getTestDetail = (target: (typeof ALL_TARGETS)[number]) => {
    if (lang === 'be' && target.testDetailBe) return target.testDetailBe;
    if (lang === 'ru') return target.testDetailRu;
    return target.testDetailEn;
  };

  return (
    <section id="targets" className="border-t border-border py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        {/* Header */}
        <div className="flex flex-col md:flex-row md:items-end md:justify-between gap-6">
          <div>
            <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
              <span>02</span>
              <span className="text-muted-foreground">//</span>
              <span>{t.runsEverywhere.heading}</span>
            </div>
            <h2 className="mt-2 font-mono text-3xl font-bold tracking-tight sm:text-4xl text-foreground">
              {t.runsEverywhere.heading}
            </h2>
            <p className="mt-2 max-w-2xl text-sm sm:text-base text-muted-foreground leading-relaxed">
              {t.runsEverywhere.sub}
            </p>
          </div>

          <div className="inline-flex items-center gap-2 rounded-lg border border-border bg-card px-3.5 py-2 font-mono text-xs text-muted-foreground shadow-xs self-start md:self-auto">
            <CheckCircle2 className="h-4 w-4 text-emerald-400" />
            <span>{t.runsEverywhere.testedBadge}</span>
          </div>
        </div>

        {/* Filter tags */}
        <div className="mt-8 flex flex-wrap gap-2">
          {[
            { id: 'all', label: t.runsEverywhere.all },
            { id: 'linux', label: t.runsEverywhere.linuxTab },
            { id: 'bsd', label: t.runsEverywhere.bsdTab },
            { id: 'openwrt', label: t.runsEverywhere.openwrtTab },
            { id: 'desktop', label: t.runsEverywhere.desktopTab },
            { id: 'wasm', label: t.runsEverywhere.wasmTab },
          ].map((item) => (
            <button
              key={item.id}
              onClick={() => setFilter(item.id as typeof filter)}
              className={`rounded-lg border px-3 py-1.5 font-mono text-xs transition-all cursor-pointer ${
                filter === item.id
                  ? 'border-accent bg-accent/15 text-foreground font-semibold shadow-xs'
                  : 'border-border bg-card text-muted-foreground hover:border-foreground/30 hover:text-foreground'
              }`}
            >
              {item.label}
            </button>
          ))}
        </div>

        {/* Dense Grid */}
        <div className="mt-8 grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
          {filteredTargets.map((target) => (
            <SpotlightCard
              key={target.id}
              className="p-4.5 border-border/70 hover:border-accent/40 transition-all flex flex-col justify-between"
            >
              <div>
                <div className="flex items-start justify-between gap-2">
                  <div className="font-mono">
                    <span className="text-sm font-bold text-foreground">{target.os}</span>
                    <span className="ml-2 rounded bg-muted px-1.5 py-0.5 text-[11px] font-semibold text-accent">
                      {target.arch}
                    </span>
                  </div>
                  {getMethodBadge(target.testMethod)}
                </div>

                <p className="mt-2 text-xs text-muted-foreground/90 font-sans leading-relaxed line-clamp-2">
                  {getDeviceDesc(target)}
                </p>

                <div className="mt-2 text-[11px] font-mono text-muted-foreground/70">
                  {getTestDetail(target)}
                </div>
              </div>

              <div className="mt-4 pt-3 border-t border-border/60 flex items-center justify-between text-[11px] font-mono">
                <span className="text-muted-foreground truncate max-w-[170px] select-all" title={target.filename}>
                  {target.filename}
                </span>
                <a
                  href={target.downloadUrl}
                  className="inline-flex items-center gap-1 rounded bg-muted/80 px-2 py-1 text-foreground hover:bg-accent hover:text-accent-foreground transition-colors shrink-0"
                >
                  <Download className="h-3 w-3" />
                  <span>get</span>
                </a>
              </div>
            </SpotlightCard>
          ))}
        </div>

        {/* Legend */}
        <div className="mt-12 rounded-xl border border-border bg-card p-6">
          <h4 className="font-mono text-xs font-semibold text-foreground uppercase tracking-wider mb-4">
            {t.runsEverywhere.legendTitle}
          </h4>
          <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-3 font-mono text-xs text-muted-foreground">
            <div className="flex items-start gap-2">
              <span className="mt-0.5 h-2 w-2 rounded-full bg-emerald-500 shrink-0" />
              <span>
                <strong className="text-foreground">qemu:</strong> {t.runsEverywhere.qemuDesc}
              </span>
            </div>
            <div className="flex items-start gap-2">
              <span className="mt-0.5 h-2 w-2 rounded-full bg-sky-500 shrink-0" />
              <span>
                <strong className="text-foreground">real vm:</strong> {t.runsEverywhere.vmDesc}
              </span>
            </div>
            <div className="flex items-start gap-2">
              <span className="mt-0.5 h-2 w-2 rounded-full bg-amber-500 shrink-0" />
              <span>
                <strong className="text-foreground">rootfs:</strong> {t.runsEverywhere.openwrtDesc}
              </span>
            </div>
            <div className="flex items-start gap-2">
              <span className="mt-0.5 h-2 w-2 rounded-full bg-rose-500 shrink-0" />
              <span>
                <strong className="text-foreground">native:</strong> {t.runsEverywhere.runnerDesc}
              </span>
            </div>
            <div className="flex items-start gap-2">
              <span className="mt-0.5 h-2 w-2 rounded-full bg-muted-foreground shrink-0" />
              <span>
                <strong className="text-foreground">only built:</strong> {t.runsEverywhere.onlyBuiltDesc}
              </span>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
};
