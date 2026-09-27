import React from 'react';
import type { Language, PageId } from '../i18n';
import { BuildSection } from '../components/BuildSection';
import { ArrowLeft } from 'lucide-react';

interface BuildPageProps {
  lang: Language;
  onNavigate: (page: PageId) => void;
}

export const BuildPage: React.FC<BuildPageProps> = ({ lang, onNavigate }) => {
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
      <BuildSection lang={lang} />
    </div>
  );
};
