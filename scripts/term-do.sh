term-do ()  { 
    term-core "$*"
    builtin cd $(cat ~/.term-do.d/pwd)
}
