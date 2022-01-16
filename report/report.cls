%!TEX program = xelatex
\NeedsTeXFormat{LaTeX2e}[1999/12/01]
\ProvidesClass{reading}[2019/10/26 A class for English homework]

\LoadClass{article}
\DeclareOption{cn}{
    \def\useCN
}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions\relax
\RequirePackage{utils}
\RequirePackage{framed}

\bibliographystyle{acm}

% In case you need to adjust margins:
\topmargin=-0.45in
\evensidemargin=0in
\oddsidemargin=0in
\textwidth=6.5in
\textheight=9.3in
\headsep=0.25in
\parskip=.05in
\parindent=2em
\setstretch{1.3}


% Setup the header and footer
\def\title#1{\def\@title{#1} \def\titleStored{#1}}

\pagestyle{fancy}
\rhead{\small\titleStored}
\ifdefined\useCN
    \lhead{\small 第\ \thepage\ 页，共\ \protect\pageref{LastPage} 页}
\else
    \lhead{Page\ \thepage\ of\ \protect\pageref{LastPage}}
\fi
\cfoot{}
\renewcommand\headrulewidth{0.4pt}
\renewcommand\footrulewidth{0.0pt}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% manipulate the title
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\newlength\aftertitskip     \newlength\beforetitskip
\newlength\interauthorskip  \newlength\aftermaketitskip

\setlength\aftertitskip{0.1in plus 0.2in minus 0.2in}
\setlength\beforetitskip{.5in plus 0.08in minus 0.08in}
\setlength\interauthorskip{0.08in plus 0.1in minus 0.1in}
\setlength\aftermaketitskip{0.1in plus 0.1in minus 0.1in}

\def\maketitle{\par
    \begingroup
        \thispagestyle{plain}
        \def\thefootnote{\fnsymbol{footnote}}
        \def\@makefnmark{\hbox to 0pt{$^{\@thefnmark}$\hss}}
        \@maketitle \@thanks
    \endgroup
    \setcounter{footnote}{0}
    \let\maketitle\relax \let\@maketitle\relax
    \gdef\@thanks{}\gdef\@author{}\gdef\@title{}\let\thanks\relax
}

\def\@startauthor{\noindent \normalsize\bf}
\def\@endauthor{}

\def\@maketitle{\vbox{\hsize\textwidth
 \linewidth\hsize \vskip \beforetitskip
 {\begin{center} \Large\bf \@title \par \end{center}} \vskip \aftertitskip
 {\def\and{\unskip\enspace{\rm and}\enspace}%
  \def\addr{\small\ifdefined\useCN\kai\else\it\fi}%
  \def\email{\hfill\small\sc}%
  \def\name{\normalsize\bf}%
  \def\AND{\@endauthor\rm\hss \vskip \interauthorskip \@startauthor}
  \@startauthor \@author \@endauthor}

  \vskip \aftermaketitskip
}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% quote environment
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\definecolor{quote-color}{gray}{0.1}
\definecolor{quote-bg}{gray}{0.9}

\renewenvironment{quote}{%
    \def\FrameCommand{
        {\color{quote-color}\vrule width 4pt}%
        \color{quote-color}%
        \colorbox{quote-bg}
    }
    \OuterFrameSep=2pt
    \MakeFramed{\advance\hsize-\width\FrameRestore}
    \vskip.3em minus .3em\nobreak
    \leftskip=1em
    \rightskip=1em
    \ifdefined\useCN \kai \else \it \fi
}{%
    \par\nobreak\vskip.3em minus .3em
    \endMakeFramed
}

\newcommand{\source}[1]{%
    \unskip\hfil\penalty50\hskip2em
    \hbox{}\nobreak\hfil #1\parfillskip=0pt\finalhyphendemerits=0}