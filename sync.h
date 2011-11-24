/* basic syncing */
#ifndef _SYNC_h_
#define _SYNC_h_

struct track_key {	int row;
					float value;	};

struct sync_track {	struct track_key *keys;
					size_t num_keys;};
#endif

