entity = {
	-- Entity identifiers
	name = "Player",
	-- Texture
	texture = "entity_player.png",
	-- Position
	position = { x = (66 * 16), y = (22 * 16), z = 0, },
	-- Draw Offset
	draw_offset = { x = -24, y = -24, },
	-- Bounding box Dimensions
	bounding_box_dimensions = { width = 10, height = 10, },
	-- Bounding box offset
	bounding_box_offset = { x = 3, y = 3, },
	-- Stats
	health = 10.0,
	movement_speed = 60.0,
	-- Animations
	has_animations = true,
	animations = {
		number_of_animations = 8,
		animation_0 = {
			name = "Idle Down",
			number_of_frames = 1,
			frame_0 = { x = 000, y = 002, length = 0.2, },
		},
		animation_1 = {
			name = "Idle Up",
			number_of_frames = 1,
			frame_0 = { x = 001, y = 002, length = 0.2, },
		},
		animation_2 = {
			name = "Idle Left",
			number_of_frames = 1,
			frame_0 = { x = 006, y = 002, length = 0.2, },
		},
		animation_3 = {
			name = "Idle Right",
			number_of_frames = 1,
			frame_0 = { x = 008, y = 002, length = 0.2, },
		},
		animation_4 = {
			name = "Walk Down",
			number_of_frames = 2,
			frame_0 = { x = 002, y = 002, length = 0.2, },
			frame_1 = { x = 003, y = 002, length = 0.2, },
		},
		animation_5 = {
			name = "Walk Up",
			number_of_frames = 2,
			frame_0 = { x = 004, y = 002, length = 0.2, },
			frame_1 = { x = 005, y = 002, length = 0.2, },
		},
		animation_6 = {
			name = "Walk Left",
			number_of_frames = 2,
			frame_0 = { x = 006, y = 002, length = 0.2, },
			frame_1 = { x = 007, y = 002, length = 0.2, },
		},
		animation_7 = {
			name = "Walk Right",
			number_of_frames = 2,
			frame_0 = { x = 008, y = 002, length = 0.2, },
			frame_1 = { x = 009, y = 002, length = 0.2, },
		},
	},
}
