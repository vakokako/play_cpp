if status is-interactive
    # Commands to run in interactive sessions can go here
end

set -gx PATH $HOME/bin /usr/local/bin /opt/bda/bin $PATH
set -gx EDITOR micro

alias config_fish="$EDITOR $HOME/.config/fish/config.fish"
alias reload_fish="source $HOME/.config/fish/config.fish"

starship init fish | source