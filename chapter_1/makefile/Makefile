#定义变量
src=$(wildcard ./*.c) #当前目录下所有的.c文件名
objs=$(patsubst %.c, %.o, $(src)) #所有.c文件名对应的.o文件名
target=app
$(target):$(objs)
	$(CC) $(objs) -o $(target)

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm $(objs) -f
