domains="www.google.com www.gla.ac.uk www.reddit.com www.bbc.com news.sky.com www.tryhackme.com www.hackthebox.eu www.github.com www.geeksforgeeks.org www.portswigger.net www.moodle.gla.ac.uk www.csperkins.org www.facebook.com www.twitter.com www.spotify.com www.discord.com www.stackoverflow.com"

parse_trace () {
	traceroute -q 1 -n $1 | sed '/\*/d' | sed '/to/d' | awk 'NR>1{print "\"" f "\" -- \"" $2 "\""} {f=$2}'
}
export -f parse_trace

# { echo "graph routertopology {" && ./dnslookup $domains | awk '{print $3}' | sed '/:/d' | xargs -d '\n' -I "%" -n 1 bash -c 'parse_trace "%"' | sort | uniq && echo "}"; } > router-4.dot
{ echo "graph routertopology {" && ./dnslookup $domains | awk '{print $3}' | sed '/\./d' | xargs -d '\n' -I "%" -n 1 bash -c 'parse_trace "%"' | sort | uniq && echo "}"; } > router-6.dot
dot -T pdf router-4.dot -o router-topology-v4.pdf
dot -T pdf router-topology-v6.dot -o router-topology-v6.pdf
