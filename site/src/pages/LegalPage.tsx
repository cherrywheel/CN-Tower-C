import React from 'react';
import type { Language, PageId } from '../i18n';
import { T } from '../i18n';
import SpotlightCard from '../components/SpotlightCard';
import { Scale, ShieldCheck, FileText, ArrowLeft, ExternalLink } from 'lucide-react';
import { GithubIcon } from '../components/Icons';

interface LegalPageProps {
  lang: Language;
  onNavigate: (page: PageId) => void;
}

export const LegalPage: React.FC<LegalPageProps> = ({ lang, onNavigate }) => {
  const t = T[lang];

  return (
    <div className="py-16 sm:py-24">
      <div className="mx-auto max-w-4xl px-4 sm:px-6 space-y-12">
        {/* Navigation & Header */}
        <div>
          <button
            onClick={() => onNavigate('home')}
            className="inline-flex items-center gap-1.5 font-mono text-xs text-muted-foreground hover:text-foreground mb-6 transition-colors cursor-pointer"
          >
            <ArrowLeft className="h-3.5 w-3.5" />
            <span>back to home</span>
          </button>

          <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
            <span>legal</span>
            <span className="text-muted-foreground">//</span>
            <span>terms & ownership</span>
          </div>

          <h1 className="mt-2 font-mono text-3xl sm:text-4xl font-bold text-foreground">
            {t.legal.heading}
          </h1>
          <p className="mt-2 text-sm sm:text-base text-muted-foreground">
            {t.legal.sub}
          </p>
        </div>

        {/* Ownership Card */}
        <SpotlightCard className="p-6 border-border space-y-4">
          <div className="flex items-center gap-2 font-mono text-sm font-bold text-foreground">
            <ShieldCheck className="h-4 w-4 text-accent" />
            <span>{t.legal.ownerTitle}</span>
          </div>
          <p className="text-xs sm:text-sm text-muted-foreground leading-relaxed">
            {t.legal.ownerText}
          </p>
          <div className="pt-2">
            <a
              href="https://github.com/cherrywheel/CN-Tower-C"
              target="_blank"
              rel="noopener noreferrer"
              className="inline-flex items-center gap-1.5 font-mono text-xs text-accent hover:underline"
            >
              <GithubIcon className="h-3.5 w-3.5" />
              <span>cherrywheel/CN-Tower-C on GitHub</span>
              <ExternalLink className="h-3 w-3" />
            </a>
          </div>
        </SpotlightCard>

        {/* License */}
        <SpotlightCard className="p-6 border-border space-y-4">
          <div className="flex items-center gap-2 font-mono text-sm font-bold text-foreground">
            <FileText className="h-4 w-4 text-accent" />
            <span>{t.legal.licenseTitle}</span>
          </div>
          <p className="text-xs sm:text-sm text-muted-foreground leading-relaxed">
            {t.legal.licenseText}
          </p>
          <div className="rounded-lg bg-muted/60 p-4 font-mono text-xs text-muted-foreground border border-border/70 overflow-x-auto">
            <p className="font-bold text-foreground">MIT License</p>
            <p className="mt-1">Copyright (c) 2026 cherrywheel</p>
            <p className="mt-2">
              Permission is hereby granted, free of charge, to any person obtaining a copy
              of this software and associated documentation files (the "Software"), to deal
              in the Software without restriction...
            </p>
          </div>
        </SpotlightCard>

        {/* Trademark Disclaimer */}
        <SpotlightCard className="p-6 border-border space-y-4">
          <div className="flex items-center gap-2 font-mono text-sm font-bold text-foreground">
            <Scale className="h-4 w-4 text-accent" />
            <span>{t.legal.disclaimerTitle}</span>
          </div>
          <p className="text-xs sm:text-sm text-muted-foreground leading-relaxed">
            {t.legal.disclaimerText}
          </p>
        </SpotlightCard>

        {/* Content & Safety */}
        <div className="rounded-xl border border-border bg-card p-6 space-y-3">
          <h3 className="font-mono text-sm font-bold text-foreground">
            {t.legal.contentTitle}
          </h3>
          <p className="text-xs sm:text-sm text-muted-foreground leading-relaxed">
            {t.legal.contentText}
          </p>
        </div>
      </div>
    </div>
  );
};
