	// I call it butt-load function
    public void StartContraption()
	{
		m_broken = false;
		m_stopTimer = 0f;
		m_parts = new List<BasePart>(GetComponentsInChildren<BasePart>());
		m_ropes.Clear();
		m_powerConsumption = 0f;
		m_enginesAmount = 0;

		if (m_hasTurboCharge)
		{
			m_enginePowerFactor = WPFMonoBehaviour.gameData.m_turboChargePowerFactor;
		}
		foreach (BasePart part in m_parts)
		{
			// Snap to grid
			Vector3 localPosition = part.transform.localPosition;
			int x = Mathf.RoundToInt(localPosition.x);
			int y = Mathf.RoundToInt(localPosition.y);
			SetPartPos(x, y, part);
			if (part.enclosedInto != null)
			{
				SetPartPos(x, y, part.enclosedInto);
			}

			// ??????
			if (part.IsIntegralPart())
			{
				m_integralParts.Add(part);
			}

			// Tag
			part.gameObject.tag = "Contraption";
			for (int i = 0; i < part.transform.childCount; i++)
			{
				part.transform.GetChild(i).gameObject.tag = "Contraption";
			}

			// Camera target
			int @int = INSettings.GetInt(INFeature.CameraTargetPartType);
			if (part.m_partType == ((SortedPartType)@int).ToPartType())
			{
				m_cameraTarget = part;
			}

			// Pig part (idk why)
			if (part.m_partType == BasePart.PartType.Pig)
			{
				m_pig = part;
			}

			// Contraption power consumption
			m_powerConsumption += part.m_powerConsumption;
		}

		// Attach to contraption (I guess)
		foreach (BasePart part2 in m_parts)
		{
			int coordX = part2.m_coordX;
			int coordY = part2.m_coordY;
			m_contraptionDataSet.AddPart(coordX, coordY, (int)part2.m_partType, part2.customPartIndex, part2.m_gridRotation, part2.m_flipped);
			part2.contraption = this;
			part2.EnsureRigidbody();
		}

		// Joint time
		foreach (BasePart part3 in m_parts)
		{
			int coordX2 = part3.m_coordX;
			int coordY2 = part3.m_coordY;
			BasePart.JointConnectionDirection customJointConnectionDirection = part3.GetCustomJointConnectionDirection();
			if (part3.m_jointConnectionType != 0)
			{
				// NVM: part at X + 1, Y and X, Y - 1 are only ones seemed to be connected
				// Each part has two joints connected to each other parts
				BasePart basePart = FindPartAt(coordX2 + 1, coordY2, part3);
				BasePart basePart2 = FindPartAt(coordX2, coordY2 - 1, part3);

				// This is just a basic checks with direction don't worry
				if (CanConnectTo(part3, basePart, BasePart.Direction.Right))
				{
					// Custom doesn't exist
					// Unless your name is rope
					BasePart.JointConnectionDirection customJointConnectionDirection2 = basePart.GetCustomJointConnectionDirection();
					if (customJointConnectionDirection == BasePart.JointConnectionDirection.Right || customJointConnectionDirection == BasePart.JointConnectionDirection.LeftAndRight)
					{
						AddCustomConnectionBetweenParts(part3, basePart);
					}
					else if (customJointConnectionDirection2 == BasePart.JointConnectionDirection.Left || customJointConnectionDirection2 == BasePart.JointConnectionDirection.LeftAndRight)
					{
						AddCustomConnectionBetweenParts(basePart, part3);
					}
					else
					{
						AddFixedJoint(part3, basePart);
					}
				}
				if (CanConnectTo(part3, basePart2, BasePart.Direction.Down))
				{
					BasePart.JointConnectionDirection customJointConnectionDirection3 = basePart2.GetCustomJointConnectionDirection();
					if (customJointConnectionDirection == BasePart.JointConnectionDirection.Down || customJointConnectionDirection == BasePart.JointConnectionDirection.UpAndDown)
					{
						AddCustomConnectionBetweenParts(part3, basePart2);
					}
					else if (customJointConnectionDirection3 == BasePart.JointConnectionDirection.Up || customJointConnectionDirection3 == BasePart.JointConnectionDirection.UpAndDown)
					{
						AddCustomConnectionBetweenParts(basePart2, part3);
					}
					else
					{
						AddFixedJoint(part3, basePart2);
					}
				}

				// Where NVM moment happened:
				if (INSettings.GetBool(INFeature.AllDirectionsConnection))
				{
					BasePart basePart3 = FindPartAt(coordX2 - 1, coordY2, part3);
					BasePart basePart4 = FindPartAt(coordX2, coordY2 + 1, part3);
					if (CanConnectTo(part3, basePart3, BasePart.Direction.Left))
					{
						BasePart.JointConnectionDirection customJointConnectionDirection4 = basePart3.GetCustomJointConnectionDirection();
						if (customJointConnectionDirection == BasePart.JointConnectionDirection.Left || customJointConnectionDirection == BasePart.JointConnectionDirection.LeftAndRight)
						{
							AddCustomConnectionBetweenParts(part3, basePart3);
						}
						else if (customJointConnectionDirection4 == BasePart.JointConnectionDirection.Right || customJointConnectionDirection4 == BasePart.JointConnectionDirection.LeftAndRight)
						{
							AddCustomConnectionBetweenParts(basePart3, part3);
						}
						else
						{
							AddFixedJoint(part3, basePart3);
						}
					}
					if (CanConnectTo(part3, basePart4, BasePart.Direction.Up))
					{
						BasePart.JointConnectionDirection customJointConnectionDirection5 = basePart4.GetCustomJointConnectionDirection();
						if (customJointConnectionDirection == BasePart.JointConnectionDirection.Up || customJointConnectionDirection == BasePart.JointConnectionDirection.UpAndDown)
						{
							AddCustomConnectionBetweenParts(part3, basePart4);
						}
						else if (customJointConnectionDirection5 == BasePart.JointConnectionDirection.Down || customJointConnectionDirection5 == BasePart.JointConnectionDirection.UpAndDown)
						{
							AddCustomConnectionBetweenParts(basePart4, part3);
						}
						else
						{
							AddFixedJoint(part3, basePart4);
						}
					}
				}

				// Rope connection
				if (part3.m_partType == BasePart.PartType.Rope && part3 is Rope)
				{
					Rope rope = (Rope)part3;
					BasePart basePart5;
					BasePart basePart6;
					if (customJointConnectionDirection == BasePart.JointConnectionDirection.LeftAndRight)
					{
						basePart5 = FindPartAt(coordX2 - 1, coordY2, part3);
						basePart6 = FindPartAt(coordX2 + 1, coordY2, part3);
					}
					else
					{
						basePart5 = FindPartAt(coordX2, coordY2 + 1, part3);
						basePart6 = FindPartAt(coordX2, coordY2 - 1, part3);
					}
					if ((bool)basePart5 && basePart5 is Rope && basePart5.m_gridRotation != part3.m_gridRotation)
					{
						basePart5 = null;
					}
					if ((bool)basePart5 && !(basePart5 is Rope) && !(basePart5 is Frame) && !(basePart5 is Kicker))
					{
						basePart5 = null;
					}
					if ((bool)basePart6 && basePart6 is Rope && basePart6.m_gridRotation != part3.m_gridRotation)
					{
						basePart6 = null;
					}
					if ((bool)basePart6 && !(basePart6 is Rope) && !(basePart6 is Frame) && !(basePart6 is Kicker))
					{
						basePart6 = null;
					}
					rope.Create(basePart5, basePart6);
					m_ropes.Add(rope);
				}
			}

			// Spring if not enclosed
			if (part3.m_partType == BasePart.PartType.Spring && part3.m_enclosedInto == null)
			{
				BasePart.Direction direction = BasePart.ConvertDirection(part3.GetCustomJointConnectionDirection());
				BasePart partAt = GetPartAt(part3, direction);
				if (!partAt || !CanConnectTo(part3, partAt, direction))
				{
					(part3 as Spring).CreateSpringBody(direction);
				}
			}
			part3.CreateCustomJoints();
			if (part3.m_partType == BasePart.PartType.Pig && (bool)WPFMonoBehaviour.levelManager && WPFMonoBehaviour.levelManager.m_disablePigCollisions && part3.m_enclosedInto != null)
			{
				part3.gameObject.layer = LayerMask.NameToLayer("NonCollidingPart");
			}
			if (part3.m_partType != BasePart.PartType.KingPig && part3.m_partType != BasePart.PartType.GoldenPig)
			{
				continue;
			}
			for (int j = 0; j <= 1; j++)
			{
				for (int k = -2; k <= 2; k += 4)
				{
					BasePart basePart7 = FindPartAt(coordX2 + k, coordY2 + j, part3);
					if (basePart7 != null && (basePart7.m_partType == BasePart.PartType.Wings || basePart7.m_partType == BasePart.PartType.MetalWing) && basePart7.collider != null)
					{
						Physics.IgnoreCollision(part3.collider, basePart7.collider);
					}
				}
				for (int l = -1; l <= 1; l++)
				{
					if (l == 0 && j == 0)
					{
						continue;
					}
					BasePart basePart8 = FindPartAt(coordX2 + l, coordY2 + j, part3);
					if (!(basePart8 != null))
					{
						continue;
					}
					if (basePart8.m_partType == BasePart.PartType.WoodenFrame || basePart8.m_partType == BasePart.PartType.MetalFrame)
					{
						if (j == 0)
						{
							AddFixedJoint(part3, basePart8);
						}
						else
						{
							Physics.IgnoreCollision(part3.collider, basePart8.collider);
						}
					}
					else if (basePart8.m_partType == BasePart.PartType.Spring && basePart8.collider != null)
					{
						Physics.IgnoreCollision(part3.collider, basePart8.collider);
					}
				}
			}
		}
		INContraption.Instance.IsRunning = true;
		INContraption.Instance.Initialize();
		for (int m = 0; m < m_parts.Count; m++)
		{
			BasePart basePart9 = m_parts[m];
			basePart9.contraption = this;
			basePart9.enabled = true;
			basePart9.Initialize();
		}
		FindConnectedComponents();
		FindComponentsConnectedByRope();
		CalculatePartPlacement();
		InitializeEngines();
		CountActiveParts();
		this.ConnectedComponentsChanged?.Invoke();
		if (m_currentGlue != 0)
		{
			MakeUnbreakable();
		}
		INContraption.Instance.PostInitialize();
		for (int n = 0; n < m_parts.Count; n++)
		{
			m_parts[n].PostInitialize();
		}
		if (m_oneShotPartAmount.TryGetValue(BasePart.PartType.Balloon, out var value))
		{
			int num = value[0];
			if (num > 0)
			{
				foreach (BasePart part4 in m_parts)
				{
					if (part4.m_partType == BasePart.PartType.Balloon)
					{
						(part4 as Balloon).ConfigureExtraBalanceJoint(1f / (float)num);
					}
				}
			}
		}
		m_running = true;
	}




	// It was all smooth sail until the nesting
	// Removed nesting as much as I can
	public bool CanConnectTo(BasePart part1, BasePart part2, BasePart.Direction direction)
	{
		bool num;
		if (part1 == null && part2 == null)
		{
			return false;
		}

		// Can connect in direction
		if (!part1.CanConnectTo(direction))
		{
			return false;
		}

		// Can connect in direction
		if (!part2.CanConnectTo(BasePart.InverseDirection(direction)))
		{
			return false;
		}

		// either one is source and non-zero connection type
		if (part1.m_jointConnectionType != 0 && part2.m_jointConnectionType != 0 && (part2.m_jointConnectionType == BasePart.JointConnectionType.Source || part1.m_jointConnectionType == BasePart.JointConnectionType.Source))
		{
			return true;
		}

		// wing checks ahhhh (by now if it did not return true, both has to be wing for further checks)
		if ((part1.m_partType != BasePart.PartType.Wings && part1.m_partType != BasePart.PartType.MetalWing) || (part2.m_partType != BasePart.PartType.Wings && part2.m_partType != BasePart.PartType.MetalWing))
		{
			return false;
		}

		// We can just ignore this (all innovation features are enabled)
		if (!INSettings.GetBool(INFeature.RotatableWing))
		{
			return direction == BasePart.Direction.Down || direction == BasePart.Direction.Up;
		}

		// Same rotation true
		if (!part1.m_gridRotation == part2.m_gridRotation)
		{
			return false;
		}

		// Idek I just gave up

		if (part1.m_gridRotation != 0 && part1.m_gridRotation != BasePart.GridRotation.Deg_180)
		{
			if (direction != BasePart.Direction.Left)
			{
				return direction == BasePart.Direction.Right;
			}
		}

		else if (direction != BasePart.Direction.Down)
		{
			num = direction == BasePart.Direction.Up;
		}

		return true;
	}





	public bool CanConnectTo(Direction direction)
	{
		switch (GetJointConnectionDirection())
		{
			case JointConnectionDirection.Any:
				return true;
			case JointConnectionDirection.Right:
				return direction == Direction.Right;
			case JointConnectionDirection.Up:
				return direction == Direction.Up;
			case JointConnectionDirection.Left:
				return direction == Direction.Left;
			case JointConnectionDirection.Down:
				return direction == Direction.Down;
			case JointConnectionDirection.LeftAndRight:
				if (direction != Direction.Left)
				{
					return direction == Direction.Right;
				}
				return true;
			case JointConnectionDirection.UpAndDown:
				if (direction != Direction.Up)
				{
					return direction == Direction.Down;
				}
				return true;
			case JointConnectionDirection.None:
				return false;
			default:
				return false;
		}
	}







	public void AddFixedJoint(BasePart part, BasePart other)
	{
		other.EnsureRigidbody();
		Joint joint;
		if (part.m_jointType == BasePart.JointType.HingeJoint || other.m_jointType == BasePart.JointType.HingeJoint)
		{
			HingeJoint hingeJoint = part.gameObject.AddComponent<HingeJoint>();
			hingeJoint.autoConfigureConnectedAnchor = false;
			hingeJoint.anchor = part.transform.InverseTransformPoint(other.transform.position) * 0.5f;
			hingeJoint.connectedAnchor = other.transform.InverseTransformPoint(part.transform.position) * 0.5f;
			JointLimits limits = hingeJoint.limits;
			limits.min = -0.1f;
			limits.max = 0.1f;
			hingeJoint.limits = limits;
			hingeJoint.axis = Vector3.forward;
			hingeJoint.useLimits = true;
			joint = hingeJoint;
		}
		else
		{
			joint = part.gameObject.AddComponent<FixedJoint>();
		}
		joint.connectedBody = other.rigidbody;
		joint.enablePreprocessing = part.JointPreprocessing && other.JointPreprocessing;
		float jointConnectionStrength = GetJointConnectionStrength(part.GetJointConnectionStrength());
		float jointConnectionStrength2 = GetJointConnectionStrength(other.GetJointConnectionStrength());
		float breakForce = jointConnectionStrength + jointConnectionStrength2;
		joint.breakForce = breakForce;
		AddJointToMap(part, other, joint);
	}





	public void AddCustomConnectionBetweenParts(BasePart part1, BasePart part2)
	{
		part2.EnsureRigidbody();
		Joint joint = part1.CustomConnectToPart(part2);
		AddJointToMap(part1, part2, joint);
	}