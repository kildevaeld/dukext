

src/timers_data.h: modules/timers.js
	uglifyjs --compress -- $^ > timers.js
	xxd -i timers.js > $@ 
	rm timers.js

clean:
	rm -f src/timers_data.h