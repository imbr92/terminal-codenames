# Terminal Codenames

## Commands
- `hjkl` or arrow keys for movement
- `i` to enter insert mode (to type clue)
- `[Enter]` to guess a tile


## Customization

Swap assets/words.txt to use a custom word list

## Dependencies and Related Projects

- [notcurses](https://github.com/dankamongmen/notcurses?tab=readme-ov-file)
- [bazel](https://github.com/bazelbuild/bazel)
- Word list from [codenames](https://github.com/jbowens/codenames/tree/master)

## Known Issues (WIP)

- Old clues aren't fully cleared by newer clues of smaller length
- Old num matches aren't cleared by newer num matches of smaller length
- Old team/role aren't cleared by newer team/role of smaller length
- Need to add counter for num matches remaining (in addition to total for that turn which already exists)
- Refresh doesn't work
