/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>

#define TC(t,c) (triangle.vertexes[(t)].gl_Position[(c)])

typedef struct {
    OutVertex vertexes[3];
} triangle_t;

void computeVertexID(VertexArray& vao, uint32_t vertex, InVertex& inVertex) {
    if (vao.indexBuffer == NULL) {
        inVertex.gl_VertexID = vertex;
    }
    else {
        if (vao.indexType == IndexType::UINT32) {
            uint32_t* indexes = (uint32_t*)vao.indexBuffer;
            inVertex.gl_VertexID = indexes[vertex];
        }
        else if (vao.indexType == IndexType::UINT16) {
            uint16_t* indexes = (uint16_t*)vao.indexBuffer;
            inVertex.gl_VertexID = indexes[vertex];
        }
        else if (vao.indexType == IndexType::UINT8) {
            uint8_t* indexes = (uint8_t*)vao.indexBuffer;
            inVertex.gl_VertexID = indexes[vertex];
        }
    }
}

void readAttributes(VertexAttrib vertexAttrib[maxAttributes], InVertex& inVertex) {
    for (uint32_t i = 0; i < maxAttributes; i++) {

        uint8_t* buffer = (uint8_t*)vertexAttrib[i].bufferData;
        uint64_t stride = vertexAttrib[i].stride;
        uint64_t offset = vertexAttrib[i].offset;
        uint8_t* ptr;
        auto type = vertexAttrib[i].type;
        if (type == AttributeType::FLOAT) {
            ptr = buffer + offset + stride * inVertex.gl_VertexID;
            inVertex.attributes[i].v1 = *((float*)ptr);
        }
        else if (type == AttributeType::VEC2) {
            for (uint32_t j = 0; j < 2; j++) {
                ptr = buffer + offset + stride * inVertex.gl_VertexID + j * 4;
                inVertex.attributes[i].v2[j] = *((float*)ptr);
            }
        }
        else if (type == AttributeType::VEC3) {
            for (uint32_t j = 0; j < 3; j++) {
                ptr = buffer + offset + stride * inVertex.gl_VertexID + j * 4;
                inVertex.attributes[i].v3[j] = *((float*)ptr);
            }
        }
        else if (type == AttributeType::VEC4) {
            for (uint32_t j = 0; j < 4; j++) {
                ptr = buffer + offset + stride * inVertex.gl_VertexID + j * 4;
                inVertex.attributes[i].v4[j] = *((float*)ptr);
            }
        }
    }
}

void runVertexAssembly(InVertex& inVertex, VertexArray vao, uint32_t vertex) {
    computeVertexID(vao, vertex, inVertex);
    readAttributes(vao.vertexAttrib, inVertex);
}

void runTriangleAssembly(triangle_t* triangle, VertexArray vao, uint32_t tr, Program prg) {
    for (uint32_t vertex = 0; vertex < 3; vertex++) {
        InVertex inVertex;
        runVertexAssembly(inVertex, vao, tr + vertex);
        prg.vertexShader(triangle->vertexes[vertex], inVertex, prg.uniforms);
    }
}

void runPerspectiveDivision(triangle_t* triangle) {
    for (uint32_t i = 0; i < 3; i++) {
        for (uint32_t j = 0; j < 3; j++) {
            triangle->vertexes[i].gl_Position[j] /= triangle->vertexes[i].gl_Position[3];
        }
    }
}

void runViewportTransformation(triangle_t* triangle, Frame frame) {
    for (uint32_t i = 0; i < 3; i++) {
        for (uint32_t j = 0; j < 2; j++) {
            triangle->vertexes[i].gl_Position[j] += 1;
            j == 0 ? triangle->vertexes[i].gl_Position[j] *= frame.width / 2 : \
                triangle->vertexes[i].gl_Position[j] *= frame.height / 2;
        }
    }
}

void createFragment(InFragment& inFragment, triangle_t triangle,
    glm::vec3 barycentrics, int x, int y, Program prg) {
    inFragment.gl_FragCoord.x = x + 0.5;
    inFragment.gl_FragCoord.y = y + 0.5;
    inFragment.gl_FragCoord.z = TC(0, 2) * barycentrics[0] +
        TC(1, 2) * barycentrics[1] + TC(2, 2) * barycentrics[2];

    double s = barycentrics[0] / TC(0, 3) + barycentrics[1] / TC(1, 3) + barycentrics[2] / TC(2, 3);
    barycentrics[0] /= TC(0, 3) * s;
    barycentrics[1] /= TC(1, 3) * s;
    barycentrics[2] /= TC(2, 3) * s;
    double res;
    for (int i = 0; i < maxAttributes; i++) {
        if (prg.vs2fs[i] == AttributeType::VEC3) {
            for (int j = 0; j < 3; j++) {
                res = 0;
                for (int k = 0; k < 3; k++) {
                    res += triangle.vertexes[k].attributes[i].v3[j] * barycentrics[k];
                }
                inFragment.attributes[i].v3[j] = res;
            }
        }
        else if (prg.vs2fs[i] == AttributeType::VEC4) {
            for (int j = 0; j < 4; j++) {
                res = 0;
                for (int k = 0; k < 3; k++) {
                    res += triangle.vertexes[k].attributes[i].v4[j] * barycentrics[k];
                }
                inFragment.attributes[i].v4[j] = res;
            }
        }
    }
}

double triangleArea(double x1, double y1, double x2, double y2, double x3, double y3) {
    long double AB = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    long double BC = sqrt(pow(x3 - x2, 2) + pow(y3 - y2, 2));
    long double CA = sqrt(pow(x1 - x3, 2) + pow(y1 - y3, 2));
    double half = (AB + BC + CA) / 2.0;
    double area = sqrt(half * (half - AB) * (half - BC) * (half - CA));
    return area;
}

void computeBarycentrics(triangle_t triangle, glm::vec3& barycentrics, \
    double area, int x, int y) {
    double X = (double)x + 0.5;
    double Y = (double)y + 0.5;
    double S1 = triangleArea(TC(0, 0), TC(0, 1), TC(1, 0), TC(1, 1), X, Y);
    double S2 = triangleArea(TC(1, 0), TC(1, 1), TC(2, 0), TC(2, 1), X, Y);
    double S3 = triangleArea(TC(2, 0), TC(2, 1), TC(0, 0), TC(0, 1), X, Y);
    barycentrics[0] = S2 / area;
    barycentrics[1] = S3 / area;
    barycentrics[2] = S1 / area;
}

float clampColor(float c) {
    c > 1 ? c = 1.0 : 0;
    return c;
}

void perFragmentOperations(Frame* frame, OutFragment &outFragment, int pix, float z)
{
    float r = clampColor(outFragment.gl_FragColor.r);
    float g = clampColor(outFragment.gl_FragColor.g);
    float b = clampColor(outFragment.gl_FragColor.b);
    float a = outFragment.gl_FragColor.a;

    if (a <= 1)
    {
        r = clampColor((frame->color[pix * 4] / 255.f) * (1 - a) + (r) * (a));
        g = clampColor((frame->color[(pix * 4) + 1] / 255.f) * (1 - a) + (g) * (a));
        b = clampColor((frame->color[(pix * 4) + 2] / 255.f) * (1 - a) + (b) * (a));
    }

    if (z < frame->depth[pix])
    {
        if (a > 0.5f)
        {
            frame->depth[pix] = z;
        }

        frame->color[pix * 4] = r * 255.f;
        frame->color[(pix * 4) + 1] = g * 255.f;
        frame->color[(pix * 4) + 2] = b * 255.f;
        return;
    }
}

double shoelace(glm::vec2 v[3]) {
    float leftSum = 0.0;
    float rightSum = 0.0;

    for (int i = 0; i < 3; ++i) {
        int j = (i + 1) % 3;
        leftSum += v[i].x * v[j].y;
        rightSum += v[j].x * v[i].y;
    }
    return 0.5 * (leftSum - rightSum);
}

void rasterizeTriangle(Frame frame, triangle_t triangle, Program prg, bool culling) {
    glm::vec2 v[3];
    for (int i = 0; i < 3; i++)
    {
        v[i].x = triangle.vertexes[i].gl_Position.x;
        v[i].y = triangle.vertexes[i].gl_Position.y;
    }
    if (culling && (shoelace(v) <= 0)) {
        return;
    }
    int minX = glm::min(glm::min(TC(0, 0), TC(1, 0)), TC(2, 0));
    minX < 0 ? minX = 0 : 0;

    int maxX = glm::max(glm::max(TC(0, 0), TC(1, 0)), TC(2, 0));
    maxX > frame.width - 1? maxX = frame.width - 1: 0;

    int minY = glm::min(glm::min(TC(0, 1), TC(1, 1)), TC(2, 1));
    minY < 0 ? minY = 0 : 0;

    int maxY = glm::max(glm::max(TC(0, 1), TC(1, 1)), TC(2, 1));
    maxY > frame.height - 1? maxY = frame.height - 1: 0;

    double area = triangleArea(TC(0, 0), TC(0, 1), TC(1, 0), TC(1, 1), TC(2, 0), TC(2, 1));
    glm::vec3 barycentrics;
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            computeBarycentrics(triangle, barycentrics, area, x, y);
            if (barycentrics[0] + barycentrics[1] + barycentrics[2] <= 1) {
                //putPixel(x, y, color1);
                InFragment inFragment;
                computeBarycentrics(triangle, barycentrics, area, x, y);
                createFragment(inFragment, triangle, barycentrics, x, y, prg);
                OutFragment outFragment;
                prg.fragmentShader(outFragment, inFragment, prg.uniforms);
                int pix = y * frame.width + x;
                perFragmentOperations(&frame, outFragment, pix, inFragment.gl_FragCoord.z);
            }
        }
    }
}


//! [drawImpl]
void drawImpl(GPUContext& ctx, uint32_t nofVertices) {
    (void)ctx;
    (void)nofVertices;
    /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
    /// ctx obsahuje aktuální stav grafické karty.
    /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
    /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
    for (uint32_t tr = 0; tr < nofVertices; tr += 3) {
        triangle_t triangle;
        runTriangleAssembly(&triangle, ctx.vao, tr, ctx.prg);
        runPerspectiveDivision(&triangle);
        runViewportTransformation(&triangle, ctx.frame);
        rasterizeTriangle(ctx.frame, triangle, ctx.prg, ctx.backfaceCulling);
    }
}
//! [drawImpl]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const& texture, glm::vec2 uv) {
    if (!texture.data)return glm::vec4(0.f);
    auto uv1 = glm::fract(uv);
    auto uv2 = uv1 * glm::vec2(texture.width - 1, texture.height - 1) + 0.5f;
    auto pix = glm::uvec2(uv2);
    //auto t   = glm::fract(uv2);
    glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f);
    for (uint32_t c = 0; c < texture.channels; ++c)
        color[c] = texture.data[(pix.y * texture.width + pix.x) * texture.channels + c] / 255.f;
    return color;
}

/**
 * @brief This function clears framebuffer.
 *
 * @param ctx GPUContext
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void clear(GPUContext& ctx, float r, float g, float b, float a) {
    auto& frame = ctx.frame;
    auto const nofPixels = frame.width * frame.height;
    for (size_t i = 0; i < nofPixels; ++i) {
        frame.depth[i] = 10e10f;
        frame.color[i * 4 + 0] = static_cast<uint8_t>(glm::min(r * 255.f, 255.f));
        frame.color[i * 4 + 1] = static_cast<uint8_t>(glm::min(g * 255.f, 255.f));
        frame.color[i * 4 + 2] = static_cast<uint8_t>(glm::min(b * 255.f, 255.f));
        frame.color[i * 4 + 3] = static_cast<uint8_t>(glm::min(a * 255.f, 255.f));
    }
}

