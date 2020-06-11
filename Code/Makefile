# This Makefile was generated using MakeGen v1.3.1 made by Tim Håkansson
# and is licensed under MIT. Full source of the project can be found at
# https://github.com/Thraix/MakeGen
CC=@g++
CO=@g++ -o
MKDIR_P=mkdir -p
BIN=bin/
OBJPATH=$(BIN)intermediates
INCLUDES=-I src/engine/ -I ext/ -I /opt/cuda/include/ 
OBJECTS=$(OBJPATH)/CommandLine.o $(OBJPATH)/AudioChannel.o $(OBJPATH)/AudioChannelManager.o $(OBJPATH)/Sound.o $(OBJPATH)/SoundManager.o $(OBJPATH)/Driver.o $(OBJPATH)/DriverDispatcher.o $(OBJPATH)/LinearDriver.o $(OBJPATH)/RectDriver.o $(OBJPATH)/EventDispatcher.o $(OBJPATH)/Animation.o $(OBJPATH)/FrameBufferObject.o $(OBJPATH)/GlobalSceneManager.o $(OBJPATH)/ParticlePool.o $(OBJPATH)/RenderCommand.o $(OBJPATH)/SceneManager.o $(OBJPATH)/Skybox.o $(OBJPATH)/Window.o $(OBJPATH)/Atlas.o $(OBJPATH)/AtlasManager.o $(OBJPATH)/Buffer.o $(OBJPATH)/VertexArray.o $(OBJPATH)/VertexBuffer.o $(OBJPATH)/TPCamera.o $(OBJPATH)/Font.o $(OBJPATH)/FontAtlas.o $(OBJPATH)/FontContainer.o $(OBJPATH)/FontManager.o $(OBJPATH)/Button.o $(OBJPATH)/ColorPicker.o $(OBJPATH)/ColorPickerWindow.o $(OBJPATH)/Component.o $(OBJPATH)/ComponentFactory.o $(OBJPATH)/Container.o $(OBJPATH)/Divider.o $(OBJPATH)/Frame.o $(OBJPATH)/GUIProperty.o $(OBJPATH)/GUIScene.o $(OBJPATH)/GUIUtils.o $(OBJPATH)/HueSlider.o $(OBJPATH)/Label.o $(OBJPATH)/ProgressBar.o $(OBJPATH)/RadioButton.o $(OBJPATH)/RadioGroup.o $(OBJPATH)/SatValSlider.o $(OBJPATH)/SceneView.o $(OBJPATH)/Slider.o $(OBJPATH)/Style.o $(OBJPATH)/TextBox.o $(OBJPATH)/Group.o $(OBJPATH)/Layer3D.o $(OBJPATH)/EntityModel.o $(OBJPATH)/Material.o $(OBJPATH)/Mesh.o $(OBJPATH)/MeshData.o $(OBJPATH)/MeshFactory.o $(OBJPATH)/BatchRenderer.o $(OBJPATH)/BatchRenderer3D.o $(OBJPATH)/GUIRenderer.o $(OBJPATH)/Renderer3D.o $(OBJPATH)/Shader.o $(OBJPATH)/ShaderFactory.o $(OBJPATH)/CubeMap.o $(OBJPATH)/ImageFactory.o $(OBJPATH)/Texture.o $(OBJPATH)/Texture2D.o $(OBJPATH)/TextureManager.o $(OBJPATH)/Input.o $(OBJPATH)/Joystick.o $(OBJPATH)/App.o $(OBJPATH)/Log.o $(OBJPATH)/LogStream.o $(OBJPATH)/Logger.o $(OBJPATH)/Mat3.o $(OBJPATH)/Mat4.o $(OBJPATH)/Quaternion.o $(OBJPATH)/Vec2.o $(OBJPATH)/Vec4.o $(OBJPATH)/ErrorHandle.o $(OBJPATH)/HotSwapping.o $(OBJPATH)/ImageUtils.o $(OBJPATH)/Noise.o $(OBJPATH)/OBJUtils.o $(OBJPATH)/Resource.o $(OBJPATH)/UUID.o $(OBJPATH)/Utils.o $(OBJPATH)/JSON.o $(OBJPATH)/JSONArray.o $(OBJPATH)/JSONLoader.o $(OBJPATH)/JSONObject.o $(OBJPATH)/XML.o $(OBJPATH)/XMLObject.o $(OBJPATH)/main.o 
CFLAGS=$(INCLUDES) -std=c++17 -c -w 
LIBDIR=-L /opt/cuda/lib64/ 
LDFLAGS=-Wl,-rpath=/opt/cuda/lib64/ 
LIBS=$(LIBDIR) -lGL -lGLEW -lglfw -lfreeimage -lfreetype -lcuda -lcudart 
OUTPUT=$(BIN)Voxelizer.a
.PHONY: all directories rebuild clean run
all: directories $(OUTPUT)
directories: $(BIN) $(OBJPATH)
$(BIN):
	$(info Creating output directories)
	@$(MKDIR_P) $(BIN)
$(OBJPATH):
	@$(MKDIR_P) $(OBJPATH)
run: all
	@./scripts/compile_cuda.sh && ./$(OUTPUT)
rebuild: clean all
clean:
	$(info Removing intermediates)
	rm -rf $(OBJPATH)/*.o
$(OUTPUT): $(OBJECTS)
	$(info Generating output file)
	$(CO) $(OUTPUT) $(OBJECTS) $(LDFLAGS) $(LIBS)
install: all
	$(info Installing Voxelizer to /usr/bin/)
	@cp $(OUTPUT) /usr/bin/Voxelizer.a
$(OBJPATH)/CommandLine.o : src/CommandLine.cpp src/CommandLine.h src/Voxelizer.h src/CudaUtils.h
	$(info -[1%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/AudioChannel.o : src/engine/audio/AudioChannel.cpp src/engine/audio/AudioChannel.h
	$(info -[2%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/AudioChannelManager.o : src/engine/audio/AudioChannelManager.cpp src/engine/audio/AudioChannelManager.h
	$(info -[3%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Sound.o : src/engine/audio/Sound.cpp src/engine/audio/Sound.h
	$(info -[4%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/SoundManager.o : src/engine/audio/SoundManager.cpp src/engine/audio/SoundManager.h
	$(info -[5%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Driver.o : src/engine/drivers/Driver.cpp src/engine/drivers/Driver.h
	$(info -[6%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/DriverDispatcher.o : src/engine/drivers/DriverDispatcher.cpp src/engine/drivers/DriverDispatcher.h
	$(info -[7%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/LinearDriver.o : src/engine/drivers/LinearDriver.cpp src/engine/drivers/LinearDriver.h
	$(info -[8%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/RectDriver.o : src/engine/drivers/RectDriver.cpp src/engine/drivers/RectDriver.h
	$(info -[9%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/EventDispatcher.o : src/engine/event/EventDispatcher.cpp src/engine/event/EventDispatcher.h
	$(info -[10%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Animation.o : src/engine/graphics/Animation.cpp src/engine/graphics/Animation.h
	$(info -[11%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/FrameBufferObject.o : src/engine/graphics/FrameBufferObject.cpp src/engine/graphics/FrameBufferObject.h
	$(info -[12%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/GlobalSceneManager.o : src/engine/graphics/GlobalSceneManager.cpp src/engine/graphics/GlobalSceneManager.h
	$(info -[13%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ParticlePool.o : src/engine/graphics/ParticlePool.cpp
	$(info -[15%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/RenderCommand.o : src/engine/graphics/RenderCommand.cpp src/engine/graphics/RenderCommand.h
	$(info -[16%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/SceneManager.o : src/engine/graphics/SceneManager.cpp src/engine/graphics/SceneManager.h
	$(info -[17%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Skybox.o : src/engine/graphics/Skybox.cpp src/engine/graphics/Skybox.h
	$(info -[18%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Window.o : src/engine/graphics/Window.cpp src/engine/graphics/Window.h
	$(info -[19%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Atlas.o : src/engine/graphics/atlas/Atlas.cpp src/engine/graphics/atlas/Atlas.h
	$(info -[20%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/AtlasManager.o : src/engine/graphics/atlas/AtlasManager.cpp src/engine/graphics/atlas/AtlasManager.h
	$(info -[21%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Buffer.o : src/engine/graphics/buffers/Buffer.cpp src/engine/graphics/buffers/Buffer.h
	$(info -[22%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/VertexArray.o : src/engine/graphics/buffers/VertexArray.cpp src/engine/graphics/buffers/VertexArray.h
	$(info -[23%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/VertexBuffer.o : src/engine/graphics/buffers/VertexBuffer.cpp src/engine/graphics/buffers/VertexBuffer.h
	$(info -[24%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/TPCamera.o : src/engine/graphics/cameras/TPCamera.cpp src/engine/graphics/cameras/TPCamera.h
	$(info -[25%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Font.o : src/engine/graphics/fonts/Font.cpp src/engine/graphics/fonts/Font.h
	$(info -[26%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/FontAtlas.o : src/engine/graphics/fonts/FontAtlas.cpp src/engine/graphics/fonts/FontAtlas.h
	$(info -[27%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/FontContainer.o : src/engine/graphics/fonts/FontContainer.cpp src/engine/graphics/fonts/FontContainer.h
	$(info -[29%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/FontManager.o : src/engine/graphics/fonts/FontManager.cpp src/engine/graphics/fonts/FontManager.h
	$(info -[30%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Button.o : src/engine/graphics/gui/Button.cpp src/engine/graphics/gui/Button.h
	$(info -[31%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ColorPicker.o : src/engine/graphics/gui/ColorPicker.cpp src/engine/graphics/gui/ColorPicker.h
	$(info -[32%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ColorPickerWindow.o : src/engine/graphics/gui/ColorPickerWindow.cpp src/engine/graphics/gui/ColorPickerWindow.h
	$(info -[33%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Component.o : src/engine/graphics/gui/Component.cpp src/engine/graphics/gui/Component.h
	$(info -[34%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ComponentFactory.o : src/engine/graphics/gui/ComponentFactory.cpp src/engine/graphics/gui/ComponentFactory.h
	$(info -[35%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Container.o : src/engine/graphics/gui/Container.cpp src/engine/graphics/gui/Container.h
	$(info -[36%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Divider.o : src/engine/graphics/gui/Divider.cpp src/engine/graphics/gui/Divider.h
	$(info -[37%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Frame.o : src/engine/graphics/gui/Frame.cpp src/engine/graphics/gui/Frame.h
	$(info -[38%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/GUIProperty.o : src/engine/graphics/gui/GUIProperty.cpp src/engine/graphics/gui/GUIProperty.h
	$(info -[39%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/GUIScene.o : src/engine/graphics/gui/GUIScene.cpp src/engine/graphics/gui/GUIScene.h
	$(info -[40%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/GUIUtils.o : src/engine/graphics/gui/GUIUtils.cpp src/engine/graphics/gui/GUIUtils.h
	$(info -[41%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/HueSlider.o : src/engine/graphics/gui/HueSlider.cpp src/engine/graphics/gui/HueSlider.h
	$(info -[43%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Label.o : src/engine/graphics/gui/Label.cpp src/engine/graphics/gui/Label.h
	$(info -[44%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ProgressBar.o : src/engine/graphics/gui/ProgressBar.cpp src/engine/graphics/gui/ProgressBar.h
	$(info -[45%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/RadioButton.o : src/engine/graphics/gui/RadioButton.cpp src/engine/graphics/gui/RadioButton.h
	$(info -[46%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/RadioGroup.o : src/engine/graphics/gui/RadioGroup.cpp src/engine/graphics/gui/RadioGroup.h
	$(info -[47%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/SatValSlider.o : src/engine/graphics/gui/SatValSlider.cpp src/engine/graphics/gui/SatValSlider.h
	$(info -[48%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/SceneView.o : src/engine/graphics/gui/SceneView.cpp src/engine/graphics/gui/SceneView.h
	$(info -[49%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Slider.o : src/engine/graphics/gui/Slider.cpp src/engine/graphics/gui/Slider.h
	$(info -[50%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Style.o : src/engine/graphics/gui/Style.cpp src/engine/graphics/gui/Style.h
	$(info -[51%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/TextBox.o : src/engine/graphics/gui/TextBox.cpp src/engine/graphics/gui/TextBox.h
	$(info -[52%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Group.o : src/engine/graphics/layers/Group.cpp src/engine/graphics/layers/Group.h
	$(info -[53%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Layer3D.o : src/engine/graphics/layers/Layer3D.cpp src/engine/graphics/layers/Layer3D.h
	$(info -[54%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/EntityModel.o : src/engine/graphics/models/EntityModel.cpp src/engine/graphics/models/EntityModel.h
	$(info -[55%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Material.o : src/engine/graphics/models/Material.cpp src/engine/graphics/models/Material.h
	$(info -[56%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Mesh.o : src/engine/graphics/models/Mesh.cpp src/engine/graphics/models/Mesh.h
	$(info -[58%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/MeshData.o : src/engine/graphics/models/MeshData.cpp src/engine/graphics/models/MeshData.h
	$(info -[59%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/MeshFactory.o : src/engine/graphics/models/MeshFactory.cpp src/engine/graphics/models/MeshFactory.h
	$(info -[60%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/BatchRenderer.o : src/engine/graphics/renderers/BatchRenderer.cpp src/engine/graphics/renderers/BatchRenderer.h
	$(info -[61%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/BatchRenderer3D.o : src/engine/graphics/renderers/BatchRenderer3D.cpp src/engine/graphics/renderers/BatchRenderer3D.h
	$(info -[62%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/GUIRenderer.o : src/engine/graphics/renderers/GUIRenderer.cpp src/engine/graphics/renderers/GUIRenderer.h
	$(info -[63%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Renderer3D.o : src/engine/graphics/renderers/Renderer3D.cpp src/engine/graphics/renderers/Renderer3D.h
	$(info -[64%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Shader.o : src/engine/graphics/shaders/Shader.cpp src/engine/graphics/shaders/Shader.h
	$(info -[65%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ShaderFactory.o : src/engine/graphics/shaders/ShaderFactory.cpp src/engine/graphics/shaders/ShaderFactory.h
	$(info -[66%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/CubeMap.o : src/engine/graphics/textures/CubeMap.cpp src/engine/graphics/textures/CubeMap.h
	$(info -[67%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ImageFactory.o : src/engine/graphics/textures/ImageFactory.cpp src/engine/graphics/textures/ImageFactory.h
	$(info -[68%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Texture.o : src/engine/graphics/textures/Texture.cpp src/engine/graphics/textures/Texture.h
	$(info -[69%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Texture2D.o : src/engine/graphics/textures/Texture2D.cpp src/engine/graphics/textures/Texture2D.h
	$(info -[70%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/TextureManager.o : src/engine/graphics/textures/TextureManager.cpp src/engine/graphics/textures/TextureManager.h
	$(info -[72%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Input.o : src/engine/input/Input.cpp src/engine/input/Input.h
	$(info -[73%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Joystick.o : src/engine/input/Joystick.cpp src/engine/input/Joystick.h
	$(info -[74%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/App.o : src/engine/internal/App.cpp src/engine/internal/App.h
	$(info -[75%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Log.o : src/engine/logging/Log.cpp src/engine/logging/Log.h
	$(info -[76%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/LogStream.o : src/engine/logging/LogStream.cpp src/engine/logging/LogStream.h
	$(info -[77%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Logger.o : src/engine/logging/Logger.cpp src/engine/logging/Logger.h
	$(info -[78%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Mat3.o : src/engine/math/Mat3.cpp src/engine/math/Mat3.h
	$(info -[79%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Mat4.o : src/engine/math/Mat4.cpp src/engine/math/Mat4.h
	$(info -[80%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Quaternion.o : src/engine/math/Quaternion.cpp src/engine/math/MathFunc.h src/engine/math/Quaternion.h src/engine/math/Vec3.h
	$(info -[81%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Vec2.o : src/engine/math/Vec2.cpp src/engine/math/Vec2.h
	$(info -[82%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Vec4.o : src/engine/math/Vec4.cpp src/engine/math/Vec4.h
	$(info -[83%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ErrorHandle.o : src/engine/utils/ErrorHandle.cpp src/engine/utils/ErrorHandle.h
	$(info -[84%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/HotSwapping.o : src/engine/utils/HotSwapping.cpp src/engine/utils/HotSwapping.h
	$(info -[86%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ImageUtils.o : src/engine/utils/ImageUtils.cpp src/engine/utils/ImageUtils.h
	$(info -[87%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Noise.o : src/engine/utils/Noise.cpp src/engine/utils/Noise.h
	$(info -[88%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/OBJUtils.o : src/engine/utils/OBJUtils.cpp src/engine/utils/OBJUtils.h
	$(info -[89%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Resource.o : src/engine/utils/Resource.cpp src/engine/utils/Resource.h
	$(info -[90%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/UUID.o : src/engine/utils/UUID.cpp src/engine/utils/UUID.h
	$(info -[91%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Utils.o : src/engine/utils/Utils.cpp src/engine/utils/Utils.h
	$(info -[92%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/JSON.o : src/engine/utils/json/JSON.cpp src/engine/utils/json/JSON.h
	$(info -[93%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/JSONArray.o : src/engine/utils/json/JSONArray.cpp src/engine/utils/json/JSONArray.h
	$(info -[94%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/JSONLoader.o : src/engine/utils/json/JSONLoader.cpp src/engine/utils/json/JSONLoader.h
	$(info -[95%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/JSONObject.o : src/engine/utils/json/JSONObject.cpp src/engine/utils/json/JSONObject.h
	$(info -[96%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/XML.o : src/engine/utils/xml/XML.cpp src/engine/utils/xml/XML.h
	$(info -[97%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/XMLObject.o : src/engine/utils/xml/XMLObject.cpp src/engine/utils/xml/XMLObject.h
	$(info -[98%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/main.o : src/main.cpp src/Axes.h src/Cam.h  src/CommandLine.h src/Voxelizer.h src/CudaUtils.h 
	$(info -[100%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
