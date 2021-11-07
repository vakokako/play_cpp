export PATH=$HOME/bin:/usr/local/bin:/opt/bda/bin:$PATH
export ZSH="/home/mvankovych/.config/zsh/oh-my-zsh"

zstyle ':omz:update' mode reminder  # just remind me to update when it's time
zstyle ':omz:update' frequency 7

# plugins=(dirhistory fzf-tab zsh-autosuggestions fast-syntax-highlighting zsh-history-substring-search autoupdate notify)
plugins=(dirhistory fzf-tab zsh-autosuggestions zsh-history-substring-search autoupdate notify)

source $ZSH/oh-my-zsh.sh

# plugins setup

## zsh-history-substring-search
bindkey '^[[A' history-substring-search-up
bindkey '^[[B' history-substring-search-down

## fzf
source $ZSH_CUSTOM/plugins/fzf/shell/key-bindings.zsh
source $ZSH_CUSTOM/plugins/fzf/shell/completion.zsh

## zsh-autosuggestions
ZSH_AUTOSUGGEST_HIGHLIGHT_STYLE="fg=#969896"

## zle settings
# 0 -- vanilla completion (abc => abc)
# 1 -- smart case completion (abc => Abc)
# 2 -- word flex completion (abc => A-big-Car)
# 3 -- full flex completion (abc => ABraCadabra)
zstyle ':completion:*' matcher-list '' 'm:{a-z\-}={A-Z\_}' 'r:[^[:alpha:]]||[[:alpha:]]=** r:|=* m:{a-z\-}={A-Z\_}' 'r:|?=** m:{a-z\-}={A-Z\_}'
zstyle ':notify:*' command-complete-timeout 5
# zle_highlight=('paste:none')


# VakoKako scripts

function cpd {
  emulate -L zsh
  print -n $PWD | clipcopy
}
function cpf {
  emulate -L zsh
  clipcopy $1
}

alias reload_zsh="exec zsh"

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

eval "$(starship init zsh)"