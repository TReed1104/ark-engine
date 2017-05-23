-- Config for the engine
config = {
	-- Window Settings
	window = {
		-- Title
		title = "ArkEngine",
		-- Tile Size
		tile_size = { x = 16, y = 16, },
		-- Window Size in Tiles
		grid_size = { x = 20, y = 15, },
		-- Window Scaler
		scale = { x = 1.0, y = 1.0, },
	},
	-- Engine Directory Settings
	directories = {
		shaders = "shaders",
		levels = "levels",
		models = "models",
		textures = "textures",
		scripts = "scripts",
	},
	-- Setup the default content, for when things are missing or fail to load.
	default_content = {
		tile = "tile.obj",
		sprite = "sprite.obj",
		texture = "default.png",
	},
	-- Controller Settings
	game_controller = {
		max_number_of_controllers = 4,
		index_of_player_controller = 0,
		thumb_stick_dead_zone = 8000,
		trigger_dead_zone = 8000,
		press_state_flag = 1,
	},
}
