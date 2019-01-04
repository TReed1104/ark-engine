map = {
	-- Name of the level
	level_name = "default",

	-- Name of the tileset to use for this level
	tileset_name = "default",

	-- Size of the map's tile grid
	tile_grid_size = { x = 25, y = 32, },

	-- Start position for the player
	player_start_grid_position = { x = 500, y = 650, },

	-- The map's tile reference grid
	map_data = {
		002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002,
		002, 002, 002, 001, 001, 001, 001, 001, 001, 001, 001, 001, 001, 001, 001, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002,
		002, 001, 001, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 001, 002, 002, 002, 002, 002, 001, 001, 001, 002, 002,
		002, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 001, 001, 002, 002, 002, 001, 001, 000, 001, 001, 002,
		002, 001, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 001, 001, 002, 001, 001, 000, 000, 000, 001, 002,
		002, 002, 001, 000, 000, 000, 001, 001, 001, 000, 000, 000, 000, 000, 000, 000, 001, 001, 001, 000, 000, 000, 000, 001, 002,
		002, 002, 001, 001, 001, 001, 001, 000, 000, 000, 000, 001, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 001, 002,
		002, 002, 002, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 001, 001, 002,
		002, 002, 002, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 001, 001, 001, 002, 002,
		002, 002, 002, 001, 000, 000, 000, 000, 000, 001, 001, 001, 001, 001, 001, 001, 001, 000, 000, 000, 000, 001, 001, 002, 002,
		002, 002, 002, 001, 001, 000, 000, 000, 000, 000, 000, 001, 002, 002, 002, 002, 001, 001, 000, 000, 000, 000, 001, 002, 002,
		002, 002, 002, 001, 001, 001, 001, 000, 000, 000, 000, 001, 002, 002, 002, 002, 002, 001, 000, 000, 000, 000, 001, 002, 002,
		002, 001, 001, 001, 000, 000, 000, 000, 000, 000, 000, 001, 001, 001, 001, 002, 002, 001, 000, 000, 000, 000, 001, 002, 002,
		002, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 008, 000, 000, 001, 002, 002, 001, 001, 008, 008, 008, 001, 002, 002,
		002, 001, 001, 000, 000, 000, 000, 000, 000, 001, 001, 001, 001, 001, 001, 002, 002, 002, 001, 008, 008, 008, 001, 002, 002,
		002, 002, 001, 000, 000, 000, 000, 000, 001, 001, 002, 002, 002, 002, 002, 002, 002, 002, 001, 000, 000, 000, 001, 001, 001,
		002, 002, 001, 000, 000, 000, 000, 000, 001, 002, 002, 002, 002, 002, 002, 002, 002, 002, 001, 000, 000, 001, 001, 000, 000,
		002, 002, 001, 000, 000, 000, 000, 000, 001, 002, 002, 002, 002, 002, 002, 002, 002, 002, 001, 000, 000, 000, 000, 000, 000,
		002, 002, 001, 000, 000, 000, 001, 001, 001, 002, 002, 002, 002, 002, 002, 002, 002, 002, 001, 001, 000, 000, 000, 000, 000,
		002, 002, 001, 001, 000, 000, 000, 001, 001, 001, 002, 002, 002, 002, 002, 002, 002, 002, 002, 001, 000, 000, 000, 000, 000,
		002, 002, 002, 001, 000, 000, 000, 000, 000, 001, 001, 002, 002, 002, 001, 001, 001, 001, 001, 001, 000, 000, 000, 000, 000,
		002, 002, 002, 001, 001, 000, 000, 000, 000, 000, 001, 001, 001, 001, 001, 000, 000, 000, 000, 001, 001, 000, 000, 000, 000,
		002, 002, 002, 002, 001, 000, 000, 000, 000, 000, 000, 000, 000, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
		002, 002, 002, 002, 001, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
		002, 002, 002, 002, 002, 001, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 001, 000, 000, 000, 000, 000, 000, 001,
		002, 002, 002, 002, 002, 002, 001, 008, 008, 001, 001, 000, 000, 000, 000, 001, 001, 001, 001, 000, 000, 000, 000, 001, 001,
		002, 002, 002, 002, 002, 002, 001, 008, 008, 001, 001, 001, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 001, 001, 002,
		002, 002, 002, 002, 002, 002, 001, 000, 001, 001, 002, 001, 001, 000, 000, 000, 000, 000, 000, 000, 000, 001, 001, 002, 002,
		002, 002, 001, 001, 001, 001, 001, 000, 001, 002, 002, 002, 001, 001, 000, 000, 000, 000, 000, 000, 001, 001, 002, 002, 002,
		002, 002, 001, 000, 000, 000, 000, 000, 001, 002, 002, 002, 002, 001, 001, 001, 001, 001, 001, 001, 001, 002, 002, 002, 002,
		002, 002, 001, 001, 001, 001, 001, 001, 001, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002,
		002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002,
	},
}
