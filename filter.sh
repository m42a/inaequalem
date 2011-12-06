#!/bin/bash
for i in $@; do
	# Change the file's extension to filtobj, adding it if no extension exists
	filt=${i%.*}.filtobj
	# Models default to black
	echo "c 0 0 0" > "$filt"
	# This changes all quads to the corresponding triangle pairs and
	# outputs only vertices and faces
	sed -n \
		-e 's/f \([0-9]\+\) \([0-9]\+\) \([0-9]\+\) \([0-9]\+\)/f \1 \2 \3\nf \3 \4 \1/' \
		-e '/^[vf]/p' \
		< "$i" >> "$filt"
done
