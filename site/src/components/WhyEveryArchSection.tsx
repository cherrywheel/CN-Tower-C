import React from 'react';
import SpotlightCard from './SpotlightCard';
import type { Language } from '../i18n';
import { T } from '../i18n';
import { ExternalLink, Cpu } from 'lucide-react';

interface WhyEveryArchProps {
  lang: Language;
}

export const WhyEveryArchSection: React.FC<WhyEveryArchProps> = ({ lang }) => {
  const t = T[lang];

  return (
    <section id="canary" className="border-t border-border py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <div className="grid gap-12 lg:grid-cols-12 items-center">
          {/* Main canary story */}
          <div className="lg:col-span-7 space-y-6">
            <div>
              <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
                <span>03</span>
                <span className="text-muted-foreground">//</span>
                <span>{t.whyEveryArch.heading}</span>
              </div>
              <h2 className="mt-2 font-mono text-3xl font-bold tracking-tight sm:text-4xl text-foreground">
                {t.whyEveryArch.heading}
              </h2>
            </div>

            <div className="space-y-4 text-sm sm:text-base text-muted-foreground leading-relaxed">
              <p className="text-foreground/90 font-medium">
                {t.whyEveryArch.p1}
              </p>
              <p>
                {t.whyEveryArch.p2}
              </p>
              <p>
                {t.whyEveryArch.p3}
              </p>
              <p>
                {t.whyEveryArch.p4}
              </p>
            </div>

            {/* Elbrus Spotlight Box */}
            <SpotlightCard className="border-border bg-card/90 p-5 mt-6">
              <div className="flex items-start gap-4">
                <div className="rounded-lg bg-accent/10 border border-accent/20 p-2.5 text-accent shrink-0 mt-0.5">
                  <Cpu className="h-5 w-5" />
                </div>
                <div className="space-y-2">
                  <div className="flex items-center justify-between">
                    <span className="font-mono text-sm font-bold text-foreground">
                      {t.whyEveryArch.elbrusHeading}
                    </span>
                    <span className="rounded bg-muted px-2 py-0.5 font-mono text-[11px] text-accent">
                      mcst lcc 1.31.05
                    </span>
                  </div>
                  <p className="text-xs text-muted-foreground leading-relaxed">
                    {t.whyEveryArch.elbrusP1} {t.whyEveryArch.elbrusP2}
                  </p>
                  <div className="pt-1 flex flex-wrap gap-4 text-xs font-mono">
                    <a
                      href="https://github.com/varyashine/e2k-toolchain"
                      target="_blank"
                      rel="noopener noreferrer"
                      className="inline-flex items-center gap-1 text-accent hover:underline"
                    >
                      <span>github: varyashine/e2k-toolchain</span>
                      <ExternalLink className="h-3 w-3" />
                    </a>
                    <a
                      href="https://varyashine.github.io/e2k-toolchain/"
                      target="_blank"
                      rel="noopener noreferrer"
                      className="inline-flex items-center gap-1 text-muted-foreground hover:text-foreground"
                    >
                      <span>varyashine.github.io/e2k-toolchain</span>
                      <ExternalLink className="h-3 w-3" />
                    </a>
                  </div>
                </div>
              </div>
            </SpotlightCard>
          </div>

          {/* Side visual sticker and quote */}
          <div className="lg:col-span-5 flex flex-col items-center">
            <div className="relative group max-w-xs sm:max-w-sm">
              <div className="absolute -inset-1 rounded-3xl bg-accent/15 blur-xl transition duration-500 group-hover:opacity-100" />
              <div className="relative rounded-2xl border border-border bg-card p-6 shadow-xl text-center space-y-4">
                <img
                  src="./sticker.webp"
                  alt="cn tower sticker"
                  className="mx-auto h-48 w-48 object-contain transition duration-300 group-hover:scale-105"
                />
                <div className="border-t border-border/80 pt-4 font-mono text-xs text-muted-foreground">
                  <div className="text-foreground font-semibold">
                    {lang === 'be'
                      ? 'canary ў чыстым выглядзе'
                      : lang === 'ru'
                      ? 'canary в чистом виде'
                      : 'a canary for your weird box'}
                  </div>
                  <div className="mt-1 text-[11px]">
                    {lang === 'be'
                      ? 'калі дайшло да перамогі — працуе ўсё'
                      : lang === 'ru'
                      ? 'если дошло до победы — работает всё'
                      : 'if it reaches victory, everything works'}
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
};
