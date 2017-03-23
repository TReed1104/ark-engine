config = {
	-- Tile Size
	tile_size = { x = 16, y = 16, },

	-- Window Size in Tiles
	window_grid = { x = 20, y = 15, },

	-- Window Scaler
	window_scale = { x = 1.0, y = 1.0, },

	-- Content Directory names
	shaders_directory = "shaders",
	levels_directory = "levels",
	models_directory = "models",
	textures_directory = "textures",
	scripts_directory = "scripts",
	
	-- Controller Setup
	game_controller = {
		max_number_of_controllers = 4,
		index_of_player_controller = 0,
		thumb_stick_dead_zone = 8000,
		trigger_dead_zone = 8000,
		press_state_flag = 1,
	},
}