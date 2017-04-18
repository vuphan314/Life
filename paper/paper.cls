\LoadClass[letterpaper, 14pt]{extarticle}
% 8pt, 9pt, 10pt, 11pt, 12pt, 14pt, 17pt, 20pt

\usepackage[vmargin=1in, hmargin=0.45in]{geometry}

\setlength{\parindent}{0pt}

\usepackage{authblk}
\usepackage{natbib}
\bibliographystyle{plainnat}

\usepackage[bookmarksnumbered]{hyperref}

\usepackage{enumitem}
\setlist[enumerate]{label*=\arabic*.}

\usepackage[yyyymmdd]{datetime}

\usepackage{amsmath}
\usepackage{amsthm}
\usepackage{amssymb}

\theoremstyle{definition} % non-italic
\newtheorem{theorem}{Theorem}[subsection]
\newtheorem{proposition}[theorem]{Proposition}
\newtheorem{corollary}[theorem]{Corollary}
\newtheorem{lemma}[theorem]{Lemma}
\newtheorem{remark}[theorem]{Remark}
\newtheorem{definition}[theorem]{Definition}
\newtheorem{example}[theorem]{Example}

\usepackage[none]{hyphenat}

\usepackage{fancyhdr}
\pagestyle{fancy}

\fancyhf{}
\renewcommand{\headrulewidth}{0pt}

\usepackage{lastpage}
\rfoot{Page \thepage{} of \pageref{LastPage}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%:cr-prolog commands

\newcommand{\textdef}[1]{\textbf{#1}}

\newcommand{\set}[1]{\{#1\}}
\newcommand{\sete}{\varnothing}

\newcommand{\setu}{\cup}
\newcommand{\seti}{\cap}
\newcommand{\setd}{\setminus}
\newcommand{\sets}{\subseteq}
\newcommand{\setsp}{\subset}

\newcommand{\impl}{\Rightarrow}
\newcommand{\equi}{\equiv}

\newcommand{\negd}[1]{not\ {#1}}

\newcommand{\ind}{i}
\newcommand{\nhl}{k} % head literals
\newcommand{\nbl}{m} % body literals
\newcommand{\nel}{n} % extended literals

\newcommand{\rlseq}[2]{\n{\rl}{#1}, \ldots, \n{\rl}{#2}}

\newcommand{\when}{\longleftarrow}
\newcommand{\whencr}{\overset{+}{\when}}

\newcommand{\regRuleNoDefNegNoDot}
  {l_1\ or \ldots or\ l_\nhl \when
	l_{\nhl+1}, \ldots, l_\nbl}
\newcommand{\regrulenodefneg}{\regRuleNoDefNegNoDot.}
\newcommand{\regrule}
  {\regRuleNoDefNegNoDot,\
	\negd{l_{\nbl+1}}, \ldots, \negd{l_\nel}.}

\newcommand{\head}[1]{head(#1)}
\newcommand{\posbody}[1]{body_+(#1)}

\newcommand{\ot}[1]{{#1}'}
\newcommand{\n}[2]{{#1}_{#2}}

\newcommand{\rl}{r}
\newcommand{\rls}{R}

\newcommand{\lit}{l}
\newcommand{\lits}{S}

\newcommand{\g}[1]{G_{#1}}
\newcommand{\cy}{C}

\newcommand{\reduct}[2]{{#1}^{#2}}

\newcommand{\p}{\Pi}
\newcommand{\preg}{\p^{reg}}
\newcommand{\pcr}{\p^{cr}}

\newcommand{\appl}[1]{\alpha(#1)}
\newcommand{\applp}[1]{\preg \setu \appl{#1}}
\newcommand{\pappl}[1]{\p_{#1}}
\newcommand{\pappld}[1]{\pappl{#1} = \applp{#1}}

\newcommand{\factify}[1]{\phi(#1)}

\newcommand{\pr}{p}
\newcommand{\prs}[3]{P({#1}, {#2}, {#3})}

\newcommand{\rkt}[3]{rank({#1}, {#2}, {#3})} % ternary
\newcommand{\rkn}{f} % nullary
\newcommand{\rku}[1]{\rkn(#1)} % unary

\newcommand{\code}[1]{\texttt{#1}}

\newcommand{\ap}{\code{AS-Prolog}}
\newcommand{\aplong}{\code{Answer Set Prolog}}

\newcommand{\cp}{\code{CR-Prolog}}
\newcommand{\cplong}{\code{Consistency Restoring Prolog}}

\newcommand{\inconspreg}{$\preg$ is inconsistent}
\newcommand{\twocrrules}{$|\pcr| \ge 2$}

\newcommand{\hc}{head-cycle}
\newcommand{\hcf}{\hc-free}

\newcommand{\dn}{default-negation}
\newcommand{\dnf}{\dn-free}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%:format commands

\newcommand{\versions}
  {This version: 2017/03/19 \bigskip}

\newcommand{\myweb}{\url{https://www.myweb.ttu.edu/vuphan/}}

\newcommand{\mytitle}
  {Syntactic Conditions for Antichain Property in \cp}

\newcommand{\mg}{Dr.\ Michael Gelfond}
\newcommand{\vp}{Vu Phan}
\newcommand{\ttu}{Texas Tech University}
