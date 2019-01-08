entity = {
	-- Entity identifiers
	name = "Player",
	-- Texture
	texture = "entity_player.png",
	-- Position
	position = { x = 140, y = 30, z = 0, },
	-- Draw Offset
	draw_offset = { x = -24, y = -24, },

	-- Bounding box Dimensions
	bounding_box_dimensions = { width = 10, height = 36, },
	-- Bounding box offset
	bounding_box_offset = { x = 3, y = 4, },

	-- Crawling
	can_crawl = true,
	-- Crawling Bounding box Dimensions
	crawling_bounding_box_dimensions = { width = 10, height = 16, },
	-- Crawling Bounding box offset
	crawling_bounding_box_offset = { x = 3, y = 24, },

	-- Stats
	health = 10.0,
	movement_speed = 60.0,

	-- Animations
	has_animations = true,
	animations = {
		number_of_animations = 8,
		animation_0 = {
			name = "Idle Standing Left",
			number_of_frames = 1,
			frame_0 = { x = 001, y = 000, length = 0.2, },
		},
		animation_1 = {
			name = "Idle Standing Right",
			number_of_frames = 1,
			frame_0 = { x = 004, y = 000, length = 0.2, },
		},
		animation_2 = {
			name = "Moving Standing Left",
			number_of_frames = 2,
			frame_0 = { x = 001, y = 000, length = 0.2, },
			frame_1 = { x = 002, y = 000, length = 0.2, },
		},
		animation_3 = {
			name = "Moving Standing Right",
			number_of_frames = 2,
			frame_0 = { x = 004, y = 000, length = 0.2, },
			frame_1 = { x = 003, y = 000, length = 0.2, },
		},
		animation_4 = {
			name = "Idle Crawl Left",
			number_of_frames = 1,
			frame_0 = { x = 005, y = 000, length = 0.2, },
		},
		animation_5 = {
			name = "Idle Crawl Right",
			number_of_frames = 1,
			frame_0 = { x = 008, y = 000, length = 0.2, },
		},
		animation_6 = {
			name = "Moving Crawl Left",
			number_of_frames = 2,
			frame_0 = { x = 005, y = 000, length = 0.2, },
			frame_1 = { x = 006, y = 000, length = 0.2, },
		},
		animation_7 = {
			name = "Moving Crawl Right",
			number_of_frames = 2,
			frame_0 = { x = 008, y = 000, length = 0.2, },
			frame_1 = { x = 007, y = 000, length = 0.2, },
		},
	},
}
