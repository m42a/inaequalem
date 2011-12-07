#!/bin/bash
# Only accept 2 or more arguments
if [[ $# -lt 3 ]]; then
	exec false
fi

# Name our variables
objfile=$1
mtlfile=$2
outfile=$3

declare -A colors
index="none"
# parse the mtl file
while read -r name val; do
	if [[ "$name" == newmtl ]]; then
		# Prefix index with _ to avoid parameter substitution and conflicts
		index=_"$val"
	elif [[ "$name" == Kd ]]; then
		colors[$index]="$val"
	fi
done < "$mtlfile"

# Filter the obj file
while read -r name val; do
	if [[ "$name" == "usemtl" ]]; then
		echo c ${colors["_$val"]}
	else
		echo $name $val
	fi
done < "$objfile" | \
sed -n \
	-e 's/f \([0-9]\+\) \([0-9]\+\) \([0-9]\+\) \([0-9]\+\)/f \1 \2 \3\nf \3 \4 \1/' \
	-e '/^[vfc]/p' \
	> "$outfile"
