entity = {
	-- Entity identifiers
	name = "Player",
	-- Texture
	texture = "entity_player.png",
	-- Position
	position = { x = (2 * 16), y = (5 * 16), z = 0, },
	-- Draw Offset
	draw_offset = { x = -24, y = -24, },
	-- Bounding box Dimensions
	bounding_box_dimensions = { width = 10, height = 36, },
	-- Bounding box offset
	bounding_box_offset = { x = 3, y = 4, },
	-- Stats
	health = 10.0,
	movement_speed = 60.0,
	-- Animations
	has_animations = true,
	animations = {
		number_of_animations = 4,
		animation_0 = {
			name = "Idle Left",
			number_of_frames = 1,
			frame_0 = { x = 001, y = 000, length = 0.2, },
		},
		animation_1 = {
			name = "Idle Right",
			number_of_frames = 1,
			frame_0 = { x = 003, y = 000, length = 0.2, },
		},
		animation_2 = {
			name = "Walk Left",
			number_of_frames = 2,
			frame_0 = { x = 001, y = 000, length = 0.2, },
			frame_1 = { x = 002, y = 000, length = 0.2, },
		},
		animation_3 = {
			name = "Walk Right",
			number_of_frames = 2,
			frame_0 = { x = 003, y = 000, length = 0.2, },
			frame_1 = { x = 004, y = 000, length = 0.2, },
		},
	},
}
