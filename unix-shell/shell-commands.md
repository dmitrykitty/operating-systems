UNIX SHELL BASICS

sudo apt update 
sudo apt upgrade 
sudo apt install ...

apt search --names-only 'regex'

what the difference between cd dev and cd /dev

redirecting 
./program < text.in 1> text.out 2> text.err
or both streams in the same time 
./program 2> &1 text.out - two streans out and err to the same file 

echo used for printing for example some variables 
echo $HOME - home 
echo $? - returnio ng code of last program 

how we can get messdage aborted when our program failed? 
maybe we csan create extra shell inside our shell? 
(   ./program < text.in ) >& text.out //what does happend? we still see Aborted. 


&& and || 
&& - complete seconmd task if first return 0 
|| - complete second task if first return error 

echo "Hello" && echo "world" -> Hello world 
echo "Hello" || echo "world" -> Hello
we can use true or false  with && and || 
in bash: 
    false -> 1
    true -> 0 

and now I can make 
(   ./program < text.in || false ) >& text.out
echo $? -> 1 why? explain it

another way to do it
bin/bash -c -c "command to be completed" >& text.out - explain it too 

Test Automatization. 
we have compiled program and tests cases test{1..5}.in 

we can write simple script 
to print all in
i here in bash - lvalue, to get rvalue $i
for i in tests/*.in; do echo $i; done -> we will see all files names 

now what we can doo 
for i in tests/*.in; do echo $i; cat $i; ./program < $i; done

$PATH - path where files are saved. when we run program - everything is searched inside path firstly 

some more scripts
mkdir -p $folder && cd $folder && for i in $(seq 1 1 10); do echo $i; echo $i > file${i}.in; done

what is executable file for unix/ linux? so basicly acces mode x? if x -> file gets executable
what is ntfs and why we can change access modes from it? 

./script.sh -> bash script.sh so bash understands it's executable and run it with bash command. So we use external shell to run out script 
and that's why all our variables became invisible inside. What we can do? 
    - run in current shell source script.sh
    - add variables to export using export variable_name
how doest it work in terms of os? external shell runing in other process? 