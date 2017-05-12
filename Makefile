
build: src/timers_data.h src/events_data.h

src/timers_data.h: modules/timers.js
	uglifyjs --compress -- $^ > timers.js
	xxd -i timers.js > $@ 
	rm timers.js

src/events_data.h: modules/event-emitter.js
	uglifyjs --compress -- $^ > events.js
	xxd -i events.js > $@ 
	rm events.js

clean:
	rm -f src/timers_data.h
	rm -r src/events_data.h