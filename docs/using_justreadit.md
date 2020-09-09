# Using JustReadIT

Now that you have built the application, you will be able to use it! :+1:

## Using OCR in a classical way.

The complete OCR can be start easily, and help you by showing an UI. You can start it by just typing:

```bash
$ ./JustReadIT.exe
```

## Trainning

If you want, you can try the neural network. For it, just start the program with the -t flag.

```bash
$ ./JustReadIt -t
```
 
 Our function can also take 3 optional parameters.
 
 - `--save` (or `-s`): if specify, the code will save the data of neural networks after working.
 - `--load` (or `-l`): if specify, the code will load the saved data of neural networks before working.
 - `--fonts x` (or `-f x`): if specify, the code will set the number of font to x.
 