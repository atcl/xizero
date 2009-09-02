-- Xizero Map Format Exporter


function main()

--test if map has correct format

	if mappy.msgBox ("xizero map export", "This will export the map layers to a .mapt,.maph,mape,maps files", mappy.MMB_OKCANCEL, mappy.MMB_ICONQUESTION) == mappy.MMB_OK then

		local w = mappy.getValue(mappy.MAPWIDTH)
		local h = mappy.getValue(mappy.MAPHEIGHT)

		if (w == 0) then

			mappy.msgBox ("xizero map export", "Invalid map width. Should be: 20", mappy.MMB_OK, mappy.MMB_ICONINFO)

		else

--export terrain map

			local isok,asname = mappy.fileRequester (".", "MAPT xizero terrain map (*.mapt)", "*.MAPT", mappy.MMB_SAVE)

			if isok == mappy.MMB_OK then

				if (not (string.sub (string.lower (asname), -5) == ".mapt")) then

					asname = asname .. ".mapt"

				end

				outfile = io.open (asname, "wb")

				--todo

				outfile:close ()

			end

--export height level map

			local isok,asname = mappy.fileRequester (".", "MAPH xizero height map (*.maph)", "*.MAPH", mappy.MMB_SAVE)

			if isok == mappy.MMB_OK then

				if (not (string.sub (string.lower (asname), -5) == ".maph")) then

					asname = asname .. ".maph"

				end

				outfile = io.open (asname, "wb")

				--todo

				outfile:close ()

			end

--export entity map

			local isok,asname = mappy.fileRequester (".", "MAPE xizero entity map (*.mape)", "*.MAPE", mappy.MMB_SAVE)

			if isok == mappy.MMB_OK then

				if (not (string.sub (string.lower (asname), -5) == ".mape")) then

					asname = asname .. ".mape"

				end

				outfile = io.open (asname, "wb")

				--todo

				outfile:close ()

			end

--export special map

			local isok,asname = mappy.fileRequester (".", "MAPS xizero special map (*.maps)", "*.MAPS", mappy.MMB_SAVE)

			if isok == mappy.MMB_OK then

				if (not (string.sub (string.lower (asname), -5) == ".maps")) then

					asname = asname .. ".maps"

				end

				outfile = io.open (asname, "wb")

				--todo

				outfile:close ()

			end

		end

	end

end

 
