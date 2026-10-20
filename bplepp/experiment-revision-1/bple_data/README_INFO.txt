All the part prefabs from Assets/PrefabInstance\Part_*.prefab, material data from a predefined list and loaded sprite data are extracted to a text form from Unity.
These data are meant for use in programming and are prepared for easy parse while maintaining human readability.
Prefab data are from multiple components of the game objects, with all it's children extracted reecursively.
Each recursion appends "      " (or more depending on nestedness of sub-data) before the current line.

Common structured data types include:
  V2: 2 component vector (float x, float y);
  V3: 3 component vector (float x, float y, float z);
  V4: 4 component vector (float x, float y, float z, float w);
  Q: quaternion (float x, float y, float z, float w);
  R: bounded rectangle (float x, float y, float width, float height);

Note: some data were in form of two integers or floating point numbers as X and Y without the internal use of Vector2Int or Vector2 data type.
In that case, I have used VF2 to represent 2 floating point X and Y variables, and VI2 to represent integer counterpart.
Version with more variables (up to 4) is also used.
There is also rectangle (R) varients: RI and RF, which contains four integers or four floating point numbers: X, Y, Width and Height.

Below are the full definitions of the enumerators found during data extraction:

	enum JointType
	{
		FixedJoint = 0,
		HingeJoint = 1
	}

	enum PartTier
	{
		Regular = 0,
		Common = 1,
		Rare = 2,
		Epic = 3,
		Legendary = 4
	}

	enum PartType
	{
		Unknown = 0,
		Balloon = 1,
		Balloons2 = 2,
		Balloons3 = 3,
		Fan = 4,
		WoodenFrame = 5,
		Bellows = 6,
		CartWheel = 7,
		Basket = 8,
		Sandbag = 9,
		Pig = 10,
		Sandbag2 = 11,
		Sandbag3 = 12,
		Propeller = 13,
		Wings = 14,
		Tailplane = 15,
		Engine = 16,
		Rocket = 17,
		MetalFrame = 18,
		SmallWheel = 19,
		MetalWing = 20,
		MetalTail = 21,
		Rotor = 22,
		MotorWheel = 23,
		TNT = 24,
		EngineSmall = 25,
		EngineBig = 26,
		NormalWheel = 27,
		Spring = 28,
		Umbrella = 29,
		Rope = 30,
		CokeBottle = 31,
		KingPig = 32,
		RedRocket = 33,
		SodaBottle = 34,
		PoweredUmbrella = 35,
		Egg = 36,
		JetEngine = 37,
		ObsoleteWheel = 38,
		SpringBoxingGlove = 39,
		StickyWheel = 40,
		GrapplingHook = 41,
		Pumpkin = 42,
		Kicker = 43,
		Gearbox = 44,
		GoldenPig = 45,
		PointLight = 46,
		SpotLight = 47,
		TimeBomb = 48,
		ElectricalPart = 49,
		MAX = 50
	}

	enum AutoAlignType
	{
		None = 0,
		Rotate = 1,
		FlipVertically = 2
	}

	enum Direction
	{
		Right = 0,
		Up = 1,
		Left = 2,
		Down = 3,
		UpRight = 4,
		UpLeft = 5,
		DownLeft = 6,
		DownRight = 7
	}

	enum GridRotation
	{
		Deg_0 = 0,
		Deg_90 = 1,
		Deg_180 = 2,
		Deg_270 = 3,
		Deg_45 = 4,
		Deg_135 = 5,
		Deg_225 = 6,
		Deg_315 = 7,
		Deg_Max = 8
	}

	enum JointConnectionType
	{
		None = 0,
		Source = 1,
		Target = 2
	}

	enum JointConnectionDirection
	{
		Any = 0,
		Right = 1,
		Up = 2,
		Left = 3,
		Down = 4,
		LeftAndRight = 5,
		UpAndDown = 6,
		None = 7
	}

	enum JointConnectionStrength
	{
		Weak = 0,
		Normal = 1,
		High = 2,
		Extreme = 3,
		HighlyExtreme = 4
	}

	enum AudioManager.AudioMaterial
	{
		None = 0,
		Wood = 1,
		Metal = 2
	}

