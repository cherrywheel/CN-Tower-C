import React from 'react';
import type { Language, PageId } from '../i18n';
import { WhyEveryArchSection } from '../components/WhyEveryArchSection';
import { ArrowLeft } from 'lucide-react';

interface CanaryPageProps {
  lang: Language;
  onNavigate: (page: PageId) => void;
}

export const CanaryPage: React.FC<CanaryPageProps> = ({ lang, onNavigate }) => {
  return (
    <div className="pt-6 pb-16">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <button
          onClick={() => onNavigate('home')}
          className="inline-flex items-center gap-1.5 font-mono text-xs text-muted-foreground hover:text-foreground mb-4 transition-colors cursor-pointer"
        >
          <ArrowLeft className="h-3.5 w-3.5" />
          <span>back to home</span>
        </button>
      </div>
      <WhyEveryArchSection lang={lang} />
    </div>
  );
};
