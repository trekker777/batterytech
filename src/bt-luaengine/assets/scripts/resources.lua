-- List all resources here
--------------------------
-- addTexture(assetName)
-- addSound(assetName)
-- addAssimp(assetName)
-- addFont(tag, assetName, size, inner stroke, outer stroke, r,g,b,a)

setDefaultTextureParams("filter", "bilinear")

addTexture("textures/smiley_tex.png")
addTexture("textures/box_surface.jpg")
addTexture("textures/box_star.jpg")
addTexture("textures/btluaenginelogo.png")
addTexture("textures/minimap_atlas.png")
setTextureParams("textures/minimap_atlas.png", "repeat", false)
addTexture("textures/arrow.png")
addTexture("textures/arrow_pressed.png")
addTexture("textures/jump.png")
addTexture("textures/jump_pressed.png")
addTexture("textures/play_panel_bg.png")
addTexture("textures/particle.png")
setTextureParams("textures/particle.png", "mipmap", true)
addTexture("ui/button1_tex.png")
addTexture("ui/button1_pressed_tex.png")
addTexture("ui/button1_selected_tex.png")
addTexture("textures/circle.png")
addTexture("textures/rectangle.png")
addAssimp("models/box.bai")
addAssimp("models/star.bai")
addTexture("models/star.jpg")
setTextureParams("textures/star.jpg", "mipmap", true)
addAssimp("models/Bleep.bai")
addTexture("models/Bleep.jpg")
setTextureParams("models/Bleep.jpg", "mipmap", true)
addTexture("models/BleepBackpack.jpg")
setTextureParams("models/BleepBackpack.jpg", "mipmap", true)
-- Physics test resources
addTexture("textures/physics_bg.jpg")
addTexture("textures/crate.jpg")
addTexture("textures/rock.png")
addTexture("textures/sinewave1.png")
addTexture("textures/smokepuff.png")
addTexture("textures/pinwheel.png")
addTexture("textures/pusher.png")
addTexture("textures/space.jpg")
addAssimp("models/skybox.obj")
addTexture("models/skybox.jpg")
addAssimp("models/quad.obj")
addAssimp("models/crater.obj")
addTexture("models/craterTexture.jpg")
setTextureParams("models/craterTexture.jpg", "mipmap", true)

-- tag, assetname, size, innerstroke, outerstroke, color
-- addFont("title", "ui/digital.ttf", 100, 1, 1, 255,255,255,255)
-- addFont("test", "ui/bm-alt.fnt", 32, 0, 0, 255,255,255,255)
-- addFont("test2", "ui/bm46.fnt", 46, 0, 0, 255,255,255,255)

addSound("sounds/click.ogg")
addSound("sounds/laser_echo.ogg")
addSound("sounds/space_noises.ogg")
addSound("sounds/whoosh.ogg")

function addTestResources()
    addAssimp("models/Seymour.bai")
    addTexture("models/Seymour.jpg")
    addTexture("textures/ground.jpg")
    setTextureParams("textures/ground.jpg", "mipmap", true)
end