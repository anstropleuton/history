import nirovanWallpaperDark from "./assets/Nirovan Wallpaper Alt Dark.png";
import nirovanWallpaperLight from "./assets/Nirovan Wallpaper Alt Light.png";

export default function App() {
  return (
    <div className="min-h-screen min-w-screen bg-black overflow-clip">
      <img
        src={nirovanWallpaperDark}
        className="object-cover object-center w-full h-full absolute"
      />

      <div className="absolute flex w-full">
        <div className="w-full p-4 mx-6 my-4 rounded-lg backdrop-blur-3xl"></div>
      </div>
    </div>
  );
}
