#version 330 core
      in vec2 TexCoords;
      out vec4 color;
      uniform sampler2D screenTexture;

      //Variables for different effects
      float offset;
      float brightnessIncrement = 0.0f;
      float kernel[9];
      float factor = 0;
      vec2 offsets[9];
      vec3 sampleTex[9];
      vec3 col;

      float clamp(float toclamp)
      {
      if(toclamp > 255.0f)
      toclamp = 255.0f;
      else if (toclamp < 0.0f)
      toclamp = 0.0f;
      return toclamp / 255.0f;
      }

      void main()
      {
      offset = 0.003;
      vec4 outColour = texture(screenTexture, TexCoords);
      offsets = vec2[](
      vec2(-offset, offset),  // top-left
      vec2(0.0f,    offset),  // top-center
      vec2(offset,  offset),  // top-right
      vec2(-offset, 0.0f),    // center-left
      vec2(0.0f,    0.0f),    // center-center
      vec2(offset,  0.0f),    // center-right
      vec2(-offset, -offset), // bottom-left
      vec2(0.0f,    -offset), // bottom-center
      vec2(offset,  -offset)  // bottom-right
      );
      outColour = vec4(1.0-outColour.r,1.0-outColour.g,1.0-outColour.b, 1.0);
color = outColour;
}