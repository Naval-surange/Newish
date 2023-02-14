# Name - Naval Surange
# Roll - 2020113018

## OSN SHELL - **New**ish

compile using 

```bash
make main
```
run using

```
./newish
```


## Descriptions

- src/  : contains all the code base.
- src/builtin  : contains all the builtin functions like `cd`,`ls`,`discover`,`echo`,`pinfo` etc.
- main.c : contains while loop.
- parser.c : contains the parser.
- prompt.c : contains the code required to generate prompt
- execute.c : contains all the code required to create a new fork and execute the programmes.
- history.c : data structure used to store history.
- processes.c : data structure used to store processes information.

**NOTE** : in case of ambiguity, it mimics the behavior of bash.
