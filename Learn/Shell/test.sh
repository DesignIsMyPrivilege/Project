#!/bin/bash
:<<EOF
这里是注释部分，可以随便更改
EOF
my_name="Jq.y"
readonly my_name
message="This is a test file. Edit by \"${my_name}\"."
readonly message
echo -e "$message \n"
#显示时间
echo -e `date`

test=(0 1 2 3 4)
echo ${test[0]}
echo "数组的所有元素:${test[*]}"
echo "数组的所有元素:${test[@]}"
for a in ${test[*]}
do
  echo $a
done

length="数组元素个数:${#test[@]}"
length1="数组元素个数:${#test[*]}"
len=${#test[*]}
len1=$len
echo $length
echo $length1

#read -p "input a val:" a
#read -p "input b val:" b
#r=$[a+b]
#echo "result=${r}"
echo "数组长度的两倍："
###算术运算符###
echo `expr $len + $len1`
###关系运算符###
if [ $len -eq $len1 ]
then
  echo "$len -eq $len1:len等于len1"
else
  echo "$len -eq $len1:len不等于len1"
fi

echo "执行的文件名:$0!"
echo "第一个参数:$1"
echo "传递到脚本的参数个数:$#"
echo "脚本运行的当前进程ID号:$$"
echo "后台运行的最后一个进程的ID号:$!"
echo "显示shell使用的当前选项:$-"
echo "显示最后命令的推出状态:$?"
###输出到文件###
echo "Test" > test.txt
echo "Test2" >> test.txt
###if条件判断###
read -p "输入参数num1:" num1
read -p "输入参数num2:" num2
if [ $num1 -eq $num2 ];then
    printf "num1等于num2\n"
elif [ $num1 -gt $num2 ];then
    printf "num1大于num2\n"
else
    printf "num1小于num2\n"
fi

