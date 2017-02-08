# Signal box program

## Documenting config command
command is as follows
`config <num of sidings> <siding 0 size> ... <siding n size>`
e.g. `config 4 3 5 3 3`

The config command will have a response code of `Status <n> <Point 0 position> ... <Point n-2 position>` where n
is the number of sidings provided.
