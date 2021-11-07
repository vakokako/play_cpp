# If you come from bash you might have to change your $PATH.
export PATH=$HOME/bin:/usr/local/bin:/opt/bda/bin:$PATH

# Path to your oh-my-zsh installation.
export ZSH="/home/mvankovych/.oh-my-zsh"

# Set name of the theme to load --- if set to "random", it will
# load a random theme each time oh-my-zsh is loaded, in which case,
# to know which specific one was loaded, run: echo $RANDOM_THEME
# See https://github.com/ohmyzsh/ohmyzsh/wiki/Themes
SPACESHIP_PROMPT_FIRST_PREFIX_SHOW="true"
ZSH_THEME="spaceship"

# Set list of themes to pick from when loading at random
# Setting this variable when ZSH_THEME=random will cause zsh to load
# a theme from this variable instead of looking in $ZSH/themes/
# If set to an empty array, this variable will have no effect.
# ZSH_THEME_RANDOM_CANDIDATES=( "robbyrussell" "agnoster" )

# Uncomment the following line to use case-sensitive completion.
# CASE_SENSITIVE="true"

# Uncomment the following line to use hyphen-insensitive completion.
# Case-sensitive completion must be off. _ and - will be interchangeable.
# HYPHEN_INSENSITIVE="true"

# Uncomment one of the following lines to change the auto-update behavior
# zstyle ':omz:update' mode disabled  # disable automatic updates
# zstyle ':omz:update' mode auto      # update automatically without asking
zstyle ':omz:update' mode reminder  # just remind me to update when it's time

# Uncomment the following line to change how often to auto-update (in days).
zstyle ':omz:update' frequency 7

# Uncomment the following line if pasting URLs and other text is messed up.
# DISABLE_MAGIC_FUNCTIONS="true"

# Uncomment the following line to disable colors in ls.
# DISABLE_LS_COLORS="true"

# Uncomment the following line to disable auto-setting terminal title.
# DISABLE_AUTO_TITLE="true"

# Uncomment the following line to enable command auto-correction.
# ENABLE_CORRECTION="true"

# Uncomment the following line to display red dots whilst waiting for completion.
# You can also set it to another string to have that shown instead of the default red dots.
# e.g. COMPLETION_WAITING_DOTS="%F{yellow}waiting...%f"
# Caution: this setting can cause issues with multiline prompts in zsh < 5.7.1 (see #5765)
COMPLETION_WAITING_DOTS="true"

# Uncomment the following line if you want to disable marking untracked files
# under VCS as dirty. This makes repository status check for large repositories
# much, much faster.
# DISABLE_UNTRACKED_FILES_DIRTY="true"

# Uncomment the following line if you want to change the command execution time
# stamp shown in the history command output.
# You can set one of the optional three formats:
# "mm/dd/yyyy"|"dd.mm.yyyy"|"yyyy-mm-dd"
# or set a custom format using the strftime function format specifications,
# see 'man strftime' for details.
# HIST_STAMPS="mm/dd/yyyy"

# Would you like to use another custom folder than $ZSH/custom?
# ZSH_CUSTOM=/path/to/new-custom-folder

# Which plugins would you like to load?
# Standard plugins can be found in $ZSH/plugins/
# Custom plugins may be added to $ZSH_CUSTOM/plugins/
# Example format: plugins=(rails git textmate ruby lighthouse)
# Add wisely, as too many plugins slow down shell startup.
# plugins=(git fzf-tab zsh-autosuggestions fast-syntax-highlighting zsh-history-substring-search autoupdate zsh-autocomplete)
plugins=(git fzf-tab zsh-autosuggestions fast-syntax-highlighting zsh-history-substring-search autoupdate notify emoji)
# plugins=(git)

source $ZSH/oh-my-zsh.sh

# User configuration

# export MANPATH="/usr/local/man:$MANPATH"

# You may need to manually set your language environment
# export LANG=en_US.UTF-8

# Preferred editor for local and remote sessions
# if [[ -n $SSH_CONNECTION ]]; then
#   export EDITOR='vim'
# else
#   export EDITOR='mvim'
# fi

# Compilation flags
# export ARCHFLAGS="-arch x86_64"

# Set personal aliases, overriding those provided by oh-my-zsh libs,
# plugins, and themes. Aliases can be placed here, though oh-my-zsh
# users are encouraged to define aliases within the ZSH_CUSTOM folder.
# For a full list of active aliases, run `alias`.
#
# Example aliases
# alias zshconfig="mate ~/.zshrc"
# alias ohmyzsh="mate ~/.oh-my-zsh"

source /usr/share/doc/fzf/examples/key-bindings.zsh
source /usr/share/doc/fzf/examples/completion.zsh

# 0 -- vanilla completion (abc => abc)
# 1 -- smart case completion (abc => Abc)
# 2 -- word flex completion (abc => A-big-Car)
# 3 -- full flex completion (abc => ABraCadabra)
zstyle ':completion:*' matcher-list '' 'm:{a-z\-}={A-Z\_}' 'r:[^[:alpha:]]||[[:alpha:]]=** r:|=* m:{a-z\-}={A-Z\_}' 'r:|?=** m:{a-z\-}={A-Z\_}'
zstyle ':notify:*' command-complete-timeout 5
zle_highlight=('paste:none')

ZSH_AUTOSUGGEST_HIGHLIGHT_STYLE="fg=#969896"

eval "$(starship init zsh)"

# keybinding

OVERRIDE_KEYBINDINGS="0"

if [ $OVERRIDE_KEYBINDINGS -eq "1" ]; then
    # erasing all "defaults"
    bindkey -r "^@" # set-mark-command
    bindkey -r "^A" # beginning-of-line
    bindkey -r "^B" # backward-char
    bindkey -r "^D" # delete-char-or-list
    bindkey -r "^E" # end-of-line
    bindkey -r "^F" # forward-char
    bindkey -r "^G" # send-break
    bindkey -r "^H" # backward-delete-char
    bindkey -r "^J" # accept-line
    bindkey -r "^K" # kill-line
    bindkey -r "^L" # clear-screen
    bindkey -r "^N" # down-line-or-history
    bindkey -r "^O" # accept-line-and-down-history
    bindkey -r "^P" # up-line-or-history
    bindkey -r "^Q" # push-line
    bindkey -r "^S" # history-incremental-search-forward
    bindkey -r "^U" # kill-whole-line
    bindkey -r "^V" # quoted-insert
    bindkey -r "^W" # backward-kill-word
    bindkey -r "^X^B" # vi-match-bracket
    bindkey -r "^X^E" # edit-command-line
    bindkey -r "^X^F" # vi-find-next-char
    bindkey -r "^X^J" # vi-join
    bindkey -r "^X^K" # kill-buffer
    bindkey -r "^X^N" # infer-next-history
    bindkey -r "^X^O" # overwrite-mode
    bindkey -r "^X^R" # _read_comp
    bindkey -r "^X^U" # undo
    bindkey -r "^X^V" # vi-cmd-mode
    bindkey -r "^X^X" # exchange-point-and-mark
    bindkey -r "^X*" # expand-word
    bindkey -r "^X=" # what-cursor-position
    bindkey -r "^X?" # _complete_debug
    bindkey -r "^XC" # _correct_filename
    bindkey -r "^XG" # list-expand
    bindkey -r "^Xa" # _expand_alias
    bindkey -r "^Xc" # _correct_word
    bindkey -r "^Xd" # _list_expansions
    bindkey -r "^Xe" # _expand_word
    bindkey -r "^Xg" # list-expand
    bindkey -r "^Xh" # _complete_help
    bindkey -r "^Xm" # _most_recent_file
    bindkey -r "^Xn" # _next_tags
    bindkey -r "^Xr" # history-incremental-search-backward
    bindkey -r "^Xs" # history-incremental-search-forward
    bindkey -r "^Xt" # _complete_tag
    bindkey -r "^Xu" # undo
    bindkey -r "^X~" # _bash_list-choices
    bindkey -r "^Y" # yank
    bindkey -r "^[^D" # list-choices
    bindkey -r "^[^G" # send-break
    bindkey -r "^[^H" # backward-kill-word
    bindkey -r "^[^I" # self-insert-unmeta
    bindkey -r "^[^J" # self-insert-unmeta
    bindkey -r "^[^L" # clear-screen
    bindkey -r "^[^M" # self-insert-unmeta
    bindkey -r "^[^_" # copy-prev-word
    bindkey -r "^[ " # expand-history
    bindkey -r "^[!" # expand-history
    bindkey -r "^[\"" # quote-region
    bindkey -r "^[\$" # spell-word
    bindkey -r "^['" # quote-line
    bindkey -r "^[," # _history-complete-newer
    bindkey -r "^[-" # neg-argument
    bindkey -r "^[." # insert-last-word
    bindkey -r "^[/" # _history-complete-older
    bindkey -r "^[0" # digit-argument
    bindkey -r "^[1" # digit-argument
    bindkey -r "^[2" # digit-argument
    bindkey -r "^[3" # digit-argument
    bindkey -r "^[4" # digit-argument
    bindkey -r "^[5" # digit-argument
    bindkey -r "^[6" # digit-argument
    bindkey -r "^[7" # digit-argument
    bindkey -r "^[8" # digit-argument
    bindkey -r "^[9" # digit-argument
    bindkey -r "^[<" # beginning-of-buffer-or-history
    bindkey -r "^[>" # end-of-buffer-or-history
    bindkey -r "^[?" # which-command
    bindkey -r "^[A" # accept-and-hold
    bindkey -r "^[B" # backward-word
    bindkey -r "^[C" # capitalize-word
    bindkey -r "^[D" # kill-word
    bindkey -r "^[F" # forward-word
    bindkey -r "^[G" # get-line
    bindkey -r "^[H" # run-help
    bindkey -r "^[L" # down-case-word
    bindkey -r "^[N" # history-search-forward
    bindkey -r "^[P" # history-search-backward
    bindkey -r "^[Q" # push-line
    bindkey -r "^[S" # spell-word
    bindkey -r "^[T" # transpose-words
    bindkey -r "^[U" # up-case-word
    bindkey -r "^[W" # copy-region-as-kill

    bindkey -r "^[[2~" # overwrite-mode
    bindkey -r "^[[3;5~" # kill-word
    bindkey -r "^[[3~" # delete-char
    bindkey -r "^[[5~" # up-line-or-history
    bindkey -r "^[[6~" # down-line-or-history
    bindkey -r "^[[Z" # reverse-menu-complete
    bindkey -r "^[_" # insert-last-word
    bindkey -r "^[a" # accept-and-hold
    bindkey -r "^[b" # backward-word
    bindkey -r "^[d" # kill-word
    bindkey -r "^[f" # forward-word
    bindkey -r "^[g" # get-line
    bindkey -r "^[h" # run-help
    bindkey -r "^[l" # "ls^J"
    bindkey -r "^[m" # copy-prev-shell-word
    bindkey -r "^[n" # history-search-forward
    bindkey -r "^[p" # history-search-backward
    bindkey -r "^[q" # push-line
    bindkey -r "^[s" # spell-word
    bindkey -r "^[t" # transpose-words
    bindkey -r "^[u" # up-case-word
    bindkey -r "^[w" # kill-region
    bindkey -r "^[x" # execute-named-cmd
    bindkey -r "^[y" # yank-pop
    bindkey -r "^[z" # execute-last-named-cmd
    bindkey -r "^[|" # vi-goto-column
    bindkey -r "^[~" # _bash_complete-word
    bindkey -r "^[^?" # backward-kill-word
    bindkey -r "^_" # undo

    # bindkey -r "!"-"~" # self-insert
    # bindkey -r "\M-^@"-"\M-^?" # self-insert

    if [[ -t 0 && $- = *i* ]]
    then
    # change Ctrl+C to Ctrl+I
    stty start ''
    stty stop ''
    stty quit ''
    stty erase ''
    stty kill ''
    stty eof '' # Ctrl + D
    stty rprnt ''
    stty werase ''
    stty lnext ''
    stty discard ''
    fi

    stty intr '^q'
    stty susp '^@'

    bindkey "^K" quoted-insert # for zle
    bindkey "^H" backward-kill-word
    bindkey "^[[3;5~" kill-word
    bindkey "^D" kill-whole-line

    bindkey "^M" accept-line
    bindkey "^?" backward-delete-char
    bindkey "^[[A" up-line-or-history
    bindkey "^[[B" down-line-or-history
    bindkey "^[[C" forward-char
    bindkey "^[[D" backward-char
    bindkey "^[OA" up-line-or-beginning-search
    bindkey "^[OB" down-line-or-beginning-search
    bindkey "^[OC" forward-char
    bindkey "^[OD" backward-char
    bindkey "^[[1;5C"  forward-word
    bindkey "^[[1;5D"  backward-word
    bindkey "^[OF"  end-of-line
    bindkey "^[OH"  beginning-of-line
    bindkey " " magic-space

    # bindkey -r "^R" # fzf-history-widget
    # bindkey -r "^T" # fzf-file-widget
    # bindkey -r "^I" # fzf-completion
    # bindkey -r "^X." # fzf-tab-debug
    # bindkey -r "^[c" # fzf-cd-widget

    # bindkey -r "^[[200~" # bracketed-paste

fi

############################################################################################
# envs
# TODO: check if no gui is possible and set to `micro`
export VISUAL="code --wait"
export EDITOR="${VISUAL}"
export GIT_EDITOR="${VISUAL}"
# export PAGER="less -R"


alias git_status_all='find . -maxdepth 1 -mindepth 1 -type d -exec sh -c "(echo {} && cd {} && git status -s && echo)" \;'
alias path_absolute="readlink -m"
alias unarchive="tar xf"
alias countFiles="find . -type f | cut -d/ -f2 | sort | uniq -c"
alias sizeFolder="du -sh"
alias rename-dir="vidir -v"
alias cdl="cd && cd - ; ls"

alias bda_test_off="BDA_BUILD_EXTRA_PROJECT_OPTIONS=\"-DENABLE_TEST=OFF -DENABLE_BENCHMARK=OFF -DENABLE_DEMOS=OFF -DENABLE_INVESTIGATIONS=OFF -DENABLE_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_BENCHMARK=OFF -DBUILD_TESTING=OFF\""
alias build_bda="./bda-build-project.sh --verbose=\"20\" --build"
alias install_bda="bda_test_off build_bda"

alias vtune_bda="echo "0"|sudo tee /proc/sys/kernel/yama/ptrace_scope && bass source /opt/intel/oneapi/vtune/latest/env/vars.sh && vtune-gui"


# source $BDASOFTSRC/BDAScripts/etc/Linux-Clang-Skylake-x86_64.cfg