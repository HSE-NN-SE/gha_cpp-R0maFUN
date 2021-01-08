read task
echo task = $task
IFS=' ' read -ra task_arr <<< "$task"

num_filters=('^[o,a]d[e,i]{0,1}n.{0,1}$' '^d[w,v][a,e]$' '^tri$' '^[a-z]+[e,i]tir[e,i]$' '^p[y,i]{0,1}at$' '^s[h]{0,1}est$' '^sem$' '^vos[e,i]m$' '^d[e,i]v[y,i]{0,1}at$' '^d[e,i]s[y,i]{0,1}at$' '^[o,a]d[e,i]n[a,o]{0,1}[a-z]{0,2}[a,o]t$' '^d[w,v][e,i]n[a-z]{0,3}[a,o]t$' '^tr[i,e]n[a-z]{0,3}[a,o]t$' '^ch[e,i]tirn[a-z]{0,3}[a,o]t$' '^p[y,i]{0,1}atn[a-z]{0,3}[a,o]t$' '^s[h]{0,1}es[t,-]n[a-z]{0,3}[a,o]t$' '^s[e,i]mn[a-z]{0,3}[a,o]t$' '^v[o,a]s[e,i]mn[a-z]{0,3}[a,o]t$' '^d[e,i]v[y,i]{0,1}atn[a-z]{0,3}[a,o]t$')
decade_filters=('^d[w,v][a-z]{0,3}[a,o]t$' '^tr[a-z]{0,3}[a,o]t$' '^sor[a,o][k,c]$' '^p[y,i]{0,1}at[d]{0,1}[e,i]s[y,i]{0,1}at$' '^s[h]{0,1}est[d]{0,1}[e,i]s[y,i]{0,1}at$' '^s[i,e]m[d]{0,1}[e,i]s[y,i]{0,1}at$' '^vos[i,e]m[d]{0,1}[e,i]s[y,i]{0,1}at$' '^d[e,i]v[y,i]{0,1}ano[s,c]t[a,o]$')
hundred_filters=('^[s,c]to$' '^d[w,v][e,i]st[e,i]$' '^tr[e,i][s,c]t[a,o]$' '^ch[e,i]tir[e,i][s,c]t[a,o]$' '^p[y,i]{0,1}[a-z]{0,3}o[t,d]$' '^sh[a-z]{0,4}o[t,d]$' '^s[i,e]m.{0,2}o[t,d]$' '^v[o,a]s[i,e]m.{0,2}o[t,d]$' '^d[e,i]v[y,i]{0,1}a.{0,2}o[t,d]$' )
word_filters=('^ti.{0,3}ch.{0,2}$' '^m.+on.*$')
sign_filters=('^pl.+s$' '^m.+us$' '^[u,y]mn.+it$' '^ra.+it$' )
signs=('+' '-' '*' '/')
in_mind=0
A=0
B=0
sign='0'
for word in "${task_arr[@]}"
do
        echo $word
        found=0
        for i in ${!num_filters[@]}
        do
                if [ "$(grep -E ${num_filters[$i]} <<< "$word")" != '' ]
                then
                        let "in_mind = $in_mind+$i+1"
                        found=1
                        break
                fi
        done
        if [ "$found" = "1" ]
        then
                continue
        fi

        for i in ${!decade_filters[@]}
        do
               if [ "$(grep -E ${decade_filters[$i]} <<< "$word")" != '' ]
                then
                        let "in_mind = $in_mind+($i+2)*10"
                        found=1
                        break
                fi
        done
        if [ "$found" = "1" ]
        then
                continue
        fi

        for i in ${!hundred_filters[@]}
        do
                if [ "$(grep -E ${hundred_filters[$i]} <<< "$word")" != '' ]
                then
                        let "in_mind=$in_mind+($i+1)*100"
                        found=1
                        break
                fi
        done
        if [ "$found" = "1" ]
        then
                continue
        fi

        for i in ${!word_filters[@]}
        do
                if [ "$(grep -E ${word_filters[$i]} <<< "$word")" != '' ]
                then
                        if [ "$i" = "0" ]
                        then
                                if [ "$sign" = '0' ]
                                then
                                        let "A=$A+$in_mind*1000"
                                        let "in_mind = 0"
                                else
                                        let "B=$B+$in_mind*1000"
                                        let "in_mind = 0"
                                fi
                        else
                                if [ "$sign" = '0' ]
                                then
                                        let "A=$A+$in_mind*1000000"
                                        let "in_mind = 0"
                                else
                                        let "B=$B+$in_mind*1000000"
                                        let "in_mind = 0"
                                fi

                        fi
                        found=1
                        break
                fi
        done
        if [ "$found" = "1" ]
        then
                continue
        fi

        for i in ${!sign_filters[@]}
        do
                if [ "$(grep -E ${sign_filters[$i]} <<< "$word")" != '' ]
                then
                        let "A=$A+$in_mind"
                        sign=${signs[$i]}
                        found=1
                        let "in_mind = 0"
                        break
                fi
        done
        if [ "$found" = "1" ]
        then
                continue
        fi



done
let "B = $B + $in_mind"

echo "converted task: "$A" "$sign" "$B" "
let "result = $A $sign $B"
echo "result = $result"
