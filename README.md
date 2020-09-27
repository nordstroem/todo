# todo

# Planned features:
Build main library. Link both test and exe with it.
Backward compatibility for older serialized binaries (built in cereal?)

# Usage:
todo --add "comment"
todo --add "comment" "2019-01-02"
todo --add "comment" tomorrow --priority 1

todo --add "comment" tomorrow --priority 1 --only-today
todo --addf file.txt

todo "comment"

todo --check 2
todo --check "comment"

todo
todo --list
todo --list today
todo --list "2019-01-02"

todo --db database_location.bin
