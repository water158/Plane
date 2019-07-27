

#include "LightningBoltSprite.h"

void LightningBoltSprite::initProgram() {
    // init shader program
    {
        GLchar *vertSource =
        (GLchar *)CCString::createWithContentsOfFile(
                                                     CCFileUtils::sharedFileUtils()
                                                     ->fullPathForFilename("shaders/lightningBolt.vsh")
                                                     .c_str())->getCString();
        GLchar *fragSource =
        (GLchar *)CCString::createWithContentsOfFile(
                                                     CCFileUtils::sharedFileUtils()
                                                     ->fullPathForFilename("shaders/lightningBolt.fsh")
                                                     .c_str())->getCString();
        CGLProgramWithUnifos *pProgram = new CGLProgramWithUnifos();
        pProgram->autorelease();
        pProgram->initWithVertexShaderByteArray(vertSource, fragSource);
        // bind attribute
        pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        // link  (must after bindAttribute)
        pProgram->link();
        // get cocos2d-x build-in uniforms
        pProgram->updateUniforms();
        // get my own uniforms
        pProgram->attachUniform("u_opacity");
        // set program
        m_program = pProgram;
        m_program->retain();
        // check gl error
        CHECK_GL_ERROR_DEBUG();
    }
}

bool LightningBoltSprite::init() {
    
    //
    initProgram();
    //
    this->CCSprite::initWithFile("shaders/lightingSeg.png");
    this->setAnchorPoint(ccp(0, 0));
    ccBlendFunc blendFuncSeg = {GL_SRC_ALPHA, GL_ONE};
    this->setBlendFunc(blendFuncSeg);
    // antiAlias
    this->getTexture()->setAntiAliasTexParameters();
    // create mesh
    m_mesh = new Cmesh();
    m_mesh->autorelease();
    m_mesh->retain();
    // create indexVBO
    m_indexVBO = new CindexVBO();
    m_indexVBO->autorelease();
    m_indexVBO->retain();
    CHECK_GL_ERROR_DEBUG();
    
    return true;
}
void LightningBoltSprite::updateGenerationTimes()
{
    float lightingLen = 2 * ccpDistance(m_startPoint, m_endPoint) * sqrtf(0.25 + m_k_offset * m_k_offset);
    
    m_nGenerationTimes = 0;
    
    while (lightingLen > m_fSegmentLength)
    {
        lightingLen /= 2;
        m_nGenerationTimes ++;
    }
    
    //CCLOG("nGeneration:%d", m_nGenerationTimes);
}

// input branchSeg and get subSeges, branchSeg itself will be deleted
vector<LineSegment *> LightningBoltSprite::generateSegmentsFromBranch(LineSegment *branchSeg, int generation)
{
    if (generation <= 0)
    {
        return vector<LineSegment *>();
        CCLOG("warning:nGeneration<=0!");
    }
    
    vector<LineSegment *> segList;
    // use branchSeg as inital seg
    segList.push_back(branchSeg); // branchSeg will be deleted in the loop below
    // begin generations
    // becuase in the loop we will change (delete) branchSeg, so here we must save
    // branchSeg's branchStartGenerationID
    // in order to the loop length not changed during the loop.
    int branchStartGenerationID = branchSeg->branchStartGenerationID;
    for (int i = branchStartGenerationID + 1;
         i < branchStartGenerationID + 1 + generation; i++) {
        vector<LineSegment *> tSegList;
        // split each seg in segList
        int nseg = (int)segList.size();
        for (int j = 0; j < nseg; j++) {
            LineSegment *seg = segList[j]; // cur seg
            vector<LineSegment *> subseges =
            splitSeg(seg, i); // split seg, get sub seges
            // add sub seges to segList
            int nSubSeg = (int)subseges.size();
            for (int k = 0; k < nSubSeg; k++) {
                tSegList.push_back(subseges[k]);
            }
        }
        // clear segList
        for (int j = 0; j < nseg; j++) {
            delete segList[j];
            segList[j] = NULL;
        }
        //
        segList = tSegList;
    }
    return segList;
}

void LightningBoltSprite::genLighting()
{
    genSeges();
    genMesh();
}

void LightningBoltSprite::genSeges()
{
    // clear segList and secondaryBranchRootList
    int nSeg = (int)m_segList.size();
    
    for (int i = 0; i < nSeg; i++)
    {
        delete m_segList[i];
        m_segList[i] = NULL;
    }
    m_segList.clear();
    
    // if nGeneration==0, namely lighting is too short
    if (m_nGenerationTimes == 0)
    {
        LineSegment *branchSeg = new LineSegment(this->m_startPoint, this->m_endPoint);
        branchSeg->branchID = 0;
        branchSeg->branchStartGenerationID = 0;
        branchSeg->generationID = 0;
        branchSeg->setBranchEndPoint(this->m_endPoint);
        m_segList.push_back(branchSeg);
        m_headSeg = branchSeg;
        m_tailSeg = branchSeg;
        
        return;
    }
    
    // create inital branchSeg
    LineSegment *branchSeg = new LineSegment(this->m_startPoint, this->m_endPoint);
    branchSeg->branchID = 0;
    branchSeg->branchStartGenerationID = 0;
    branchSeg->generationID = 0;
    branchSeg->setBranchEndPoint(this->m_endPoint);
    
    // use inital branchSeg to create segList
    m_segList = generateSegmentsFromBranch(branchSeg, m_nGenerationTimes);
    
    // find head and tail seg
    m_headSeg = m_segList[0];
    m_tailSeg = m_segList[(int)m_segList.size() - 1];
    assert(m_headSeg->branchID == 0 && m_headSeg->father == NULL);
    assert(m_tailSeg->branchID == 0 && (int)m_tailSeg->children.size() == 0);
}

vector<LineSegment *> LightningBoltSprite::splitSeg(LineSegment *seg,
                                                    int generationID) {
    // CCLOG("seg->father branchID:%i",seg->branchID);
    
    //  *-------seg-------*
    //  *--seg0--*--seg1--*
    //
    vector<LineSegment *> seges;
    {
        float maxOffset = seg->getLen() * m_k_offset;
        float offset = randomInRange(-maxOffset, maxOffset);
        CCPoint midPoint =
        seg->getMidPoint() + ccpMult(seg->getRightNormal(), offset);
        LineSegment *seg0 = new LineSegment(seg->getStartPoint(), midPoint);
        LineSegment *seg1 = new LineSegment(midPoint, seg->getEndPoint());
        seg0->branchID = seg->branchID; // sub seg's branchID inherited from seg
        seg0->branchStartGenerationID = seg->branchStartGenerationID;
        seg0->setBranchEndPoint(seg->getBranchEndPoint());
        seg0->generationID = seg->generationID + 1;
        seg1->branchID = seg->branchID;
        seg1->branchStartGenerationID = seg->branchStartGenerationID;
        seg1->setBranchEndPoint(seg->getBranchEndPoint());
        seg1->generationID = seg->generationID + 1;
        // update links of seg0, seg1
        seg0->father = seg->father;
        seg0->addChildRef(seg1);
        seg1->father = seg0;
        seg1->children = seg->children;
        // update links of seg's neighbour
        if (seg->father) {
            seg->father->addChildRef(seg0);
            seg->father->removeChildRef(seg);
        }
        for (int i = 0; i < (int)seg->children.size(); i++) {
            LineSegment *child = seg->children[i];
            child->father = seg1;
        }
        // add seg0,seg2(if has),seg1 to seges, notice the order, if we keep this
        // order, we can guarantee segList[0] is headSeg and
        // segList[(int)segList.size()-1] is tailSeg.
        // note, jitter will break this law
        // add seg0 to seges
        seges.push_back(seg0);
        // calculate branch
        float p = nPow(0.6, generationID - 1); // 0.55//0.6
        if (catchProbability(p)) {
            float randomSmallAngle = randomInRange(-3, 3); //-3,3
            float lengthScale = 0.6 * nPow(1.2, seg->branchID); // 0.7//0.6
            CCPoint splitEnd =
            ccpMult(rotateVector2(seg0->getVector(), randomSmallAngle),
                    lengthScale) +
            midPoint;
            LineSegment *seg2 = new LineSegment(midPoint, splitEnd);
            seg2->branchID =
            seg->branchID + 1; // branch's branchID is seg's branchID+1
            seg2->branchStartGenerationID = generationID;
            seg2->setBranchEndPoint(seg2->getEndPoint());
            seg2->generationID = seg->generationID + 1;
            // update links of seg2
            seg2->father = seg0;
            seg0->addChildRef(seg2);
            // add seg2 to seges
            seges.push_back(seg2);
        }
        // add seg1 to seges
        seges.push_back(seg1);
    }
    return seges;
}

void LightningBoltSprite::doJitter()
{
    // generate branchList
    vector<LineSegment *> branchList;
    {
        LineSegment *seg = m_headSeg;
        while (1)
        {
            // seg must be mainBranch seg
            // see if seg's children have secondary branch
            LineSegment *nextSeg = NULL;
            int nChild = (int)seg->children.size();
            if (nChild > 1)
            {
                // have secondary branch
                for (int i = 0; i < nChild; i++)
                {
                    LineSegment *_seg = seg->children[i];
                    if (_seg->branchID == 1)
                    {
                        //_seg is secondary branch root seg
                        // make branchSeg based on _seg
                        branchList.push_back(_seg);
                    }
                    else if (_seg->branchID == 0)
                    {
                        nextSeg = _seg;
                    }
                }
            }
            else if (nChild == 1)
            {
                nextSeg = seg->children[0];
            }
            
            // if have next seg then go on, else jump out
            if (nextSeg)
            {
                seg = nextSeg;
            }
            else
            {
                break;
            }
        } // got secondaryBranchList
    }
    
    // mark branch to be regenerated
    {
        int nbranch = (int)branchList.size();
        for (int i = 0; i < nbranch; i++)
        {
            LineSegment *branchSeg = branchList[i];
        
            if (catchProbability(m_percentageOfJitterBranch))
            {
                branchSeg->setNeedRegeneration(true);
            }
            else
            {
                branchSeg->setNeedRegeneration(false);
            }
        }
    }
    
    // generate seeds
    // if a branch need not regenerate, it's seed is NULL
    vector<LineSegment *> seedList((int)branchList.size());
    {
        int nSeed = (int)seedList.size();
        for (int i = 0; i < nSeed; i++)
        {
            if (branchList[i]->getNeedRegeneration())
            {
                LineSegment *seed = new LineSegment();
                *seed = *branchList[i];
                seed->setEndPoint(seed->getBranchEndPoint());
                seed->generationID = branchList[i]->branchStartGenerationID;
                seed->children.clear();
                seedList[i] = seed;
            }
            else
            {
                seedList[i] = NULL;
            }
        }
    }
    
    // clear branches need to regenerate
    releaseNeedReGenBranch(branchList, m_segList);
    
    // generate seges for each seg in secondaryBranchList
    int nSeed = (int)seedList.size();
    for (int i = 0; i < nSeed; i++)
    {
        LineSegment *seed = seedList[i];
        
        if (seed == NULL)
        {
            continue;
        }
        
        seed->setEndPoint(seed->getStartPoint() +
                          ccpMult(rotateVector2(seed->getVector(), randomInRange(-3, 3)), 0.7));
        
        // generate seges for branchSeg
        int nGeneration_seed =
        m_nGenerationTimes - seed->branchStartGenerationID; // how many generations
        // this seed will grow
        if (nGeneration_seed <= 0)
            continue; // because nGeneration may changed, so nGeneration_seed may <= 0
        vector<LineSegment *> tSegList =
        generateSegmentsFromBranch(seed, nGeneration_seed);
        assert(tSegList[0]->father);
        assert((int)tSegList[(int)tSegList.size() - 1]->children.size() == 0);
        // add tSegList to segList
        int ntSeg = (int)tSegList.size();
        for (int j = 0; j < ntSeg; j++) {
            m_segList.push_back(tSegList[j]);
        }
    }
    genMesh();
}

void
LightningBoltSprite::releaseNeedReGenBranch(vector<LineSegment *> &branchList,
                                            vector<LineSegment *> &segList) {
    int nbranch = (int)branchList.size();
    for (int i = 0; i < nbranch; i++) {
        LineSegment *branchSeg = branchList[i];
        if (branchSeg->getNeedRegeneration()) {
            // remove its reference from its father's children
            branchSeg->father->removeChildRef(branchSeg);
            // release this whole branch
            releaseNeedReGenBranch_inn(branchSeg, segList);
        }
    }
    // remove all NULL elements in segList
    int nseg = (int)segList.size();
    for (int i = 0; i < nseg; i++) {
        if (segList[i]->getNeedRegeneration()) {
            delete segList[i];
            segList[i] = NULL;
        }
    }
    
    vector<LineSegment *> tSegList;
    for (int i = 0; i < nseg; i++) {
        if (segList[i] != NULL) {
            tSegList.push_back(segList[i]);
        }
    }
    segList = tSegList;
}

void LightningBoltSprite::releaseNeedReGenBranch_inn(
                                                     LineSegment *seg, vector<LineSegment *> &segList) {
    if (seg == NULL)
        return;
    seg->setNeedRegeneration(true);
    // same process for children
    int nChild = (int)seg->children.size();
    for (int i = 0; i < nChild; i++) {
        LineSegment *_seg = seg->children[i];
        releaseNeedReGenBranch_inn(_seg, segList);
    }
}

void LightningBoltSprite::genMesh() {
    
    // make mesh
    {
        m_mesh->vlist.clear();
        m_mesh->IDtriList.clear();
        m_mesh->texCoordList.clear();
        m_mesh->colorList.clear();
        int nSeg = (int)m_segList.size();
        for (int i = 0; i < nSeg; i++) {
            LineSegment *seg = m_segList[i];
            // calculate quad for seg
            //      v0        v3
            //       * -------*
            //       |segW/2  |
            //   end <--------- start
            //       |        |
            //       *--------*
            //      v1        v2
            const CCPoint &start = seg->getStartPoint();
            const CCPoint &end = seg->getEndPoint();
            CCPoint rightNorm = seg->getRightNormal();
            float segW = 12 * nPow(0.3, seg->branchID);
            float opacity = 1.0 * nPow(0.4, seg->branchID);
            CCPoint p0 = end + ccpMult(rightNorm, segW / 2);
            CCPoint p1 = end + ccpMult(rightNorm, -segW / 2);
            CCPoint p2 = start + ccpMult(rightNorm, -segW / 2);
            CCPoint p3 = start + ccpMult(rightNorm, segW / 2);
            {
                CCPoint mid = seg->getMidPoint();
                p0 = mid + ccpMult(p0 - mid, 8);
                p1 = mid + ccpMult(p1 - mid, 8);
                p2 = mid + ccpMult(p2 - mid, 8);
                p3 = mid + ccpMult(p3 - mid, 8);
            }
            CCPoint texCoord0 = CCPoint(0, 0);
            CCPoint texCoord1 = CCPoint(0, 1);
            CCPoint texCoord2 = CCPoint(1, 1);
            CCPoint texCoord3 = CCPoint(1, 0);
            m_mesh->vlist.push_back(Cv2(p0.x, p0.y));
            int vID0 = (int)m_mesh->vlist.size() - 1;
            m_mesh->vlist.push_back(Cv2(p1.x, p1.y));
            int vID1 = (int)m_mesh->vlist.size() - 1;
            m_mesh->vlist.push_back(Cv2(p2.x, p2.y));
            int vID2 = (int)m_mesh->vlist.size() - 1;
            m_mesh->vlist.push_back(Cv2(p3.x, p3.y));
            int vID3 = (int)m_mesh->vlist.size() - 1;
            m_mesh->texCoordList.push_back(Cv2(texCoord0.x, texCoord0.y));
            m_mesh->texCoordList.push_back(Cv2(texCoord1.x, texCoord1.y));
            m_mesh->texCoordList.push_back(Cv2(texCoord2.x, texCoord2.y));
            m_mesh->texCoordList.push_back(Cv2(texCoord3.x, texCoord3.y));
            
            m_mesh->colorList.push_back(Cv4(1, 1, 1, opacity));
            m_mesh->colorList.push_back(Cv4(1, 1, 1, opacity));
            m_mesh->colorList.push_back(Cv4(1, 1, 1, opacity));
            m_mesh->colorList.push_back(Cv4(1, 1, 1, opacity));
            
            CIDTriangle IDtri0(vID0, vID1, vID2);
            CIDTriangle IDtri1(vID0, vID2, vID3);
            m_mesh->IDtriList.push_back(IDtri0);
            m_mesh->IDtriList.push_back(IDtri1);
        }
    }
    // submit mesh to indexVBO
    {
        // submit data to VBO
        m_indexVBO->submitPos(m_mesh->vlist, GL_STREAM_DRAW); // GL_DYNAMIC_DRAW
        m_indexVBO->submitTexCoord(m_mesh->texCoordList, GL_STREAM_DRAW);
        m_indexVBO->submitColor(m_mesh->colorList, GL_STATIC_DRAW);
        m_indexVBO->submitIndex(m_mesh->IDtriList, GL_STREAM_DRAW);
        // check gl error
        CHECK_GL_ERROR_DEBUG();
    }
}

void LightningBoltSprite::setStartAndEnd(CCPoint start, CCPoint end)
{
    m_startPoint_parentSpace = start;
    m_endPoint_parentSpace = end;
    m_startPoint = CCPointApplyAffineTransform(m_startPoint_parentSpace, this->parentToNodeTransform());
    m_endPoint = CCPointApplyAffineTransform(m_endPoint_parentSpace, this->parentToNodeTransform());
    // startPoint and endPoint is changed
    
    
    // relocate all the seges in segList
    if (m_headSeg && m_tailSeg)
    {
        // move all seges let headStart equals to startPoint
        CCPoint vec_headStartToStart = m_startPoint - m_headSeg->getStartPoint();
        
        int nseg = (int)m_segList.size();
        
        for (int i = 0; i < nseg; i++)
        {
            LineSegment *seg = m_segList[i];
            
            seg->setStartPoint(seg->getStartPoint() + vec_headStartToStart);
            seg->setEndPoint(seg->getEndPoint() + vec_headStartToStart);
            
            if (seg->branchID == 1 && seg->father->branchID == 0) {
                // secondary branch root seg
                seg->setBranchEndPoint(seg->getBranchEndPoint() + vec_headStartToStart);
            }
        }
        
        // rotate all seges in segList
        CCPoint vec_headStartToTailEnd =
        m_tailSeg->getEndPoint() - m_headSeg->getStartPoint();
        CCPoint vec_headStartToEnd = this->m_endPoint - m_headSeg->getStartPoint();
        CCPoint cosA_sinA = calculateCosASinAOfVec1ToVec2(vec_headStartToTailEnd,
                                                          vec_headStartToEnd);
        float cosA = cosA_sinA.x;
        float sinA = cosA_sinA.y;
        for (int i = 0; i < nseg; i++)
        {
            LineSegment *seg = m_segList[i];
            
            seg->setStartPoint(m_headSeg->getStartPoint() +
                               rotateVector2(seg->getStartPoint() - m_headSeg->getStartPoint(),
                                             cosA,
                                             sinA));
            
            seg->setEndPoint(m_headSeg->getStartPoint() +
                             rotateVector2(seg->getEndPoint() - m_headSeg->getStartPoint(),
                                           cosA,
                                           sinA));
            
            if (seg->branchID == 1 && seg->father->branchID == 0)
            {
                // secondary branch root seg
                seg->setBranchEndPoint(m_headSeg->getStartPoint() +
                                       rotateVector2(seg->getBranchEndPoint() - m_headSeg->getStartPoint(),
                                                     cosA,
                                                     sinA));
            }
        }
        
        // scale segList, use headSeg->startPoint as the center
        float dis_headStartToEnd =
        ccpDistance(m_headSeg->getStartPoint(), this->m_endPoint);
        float dis_headStartToTailEnd =
        ccpDistance(m_headSeg->getStartPoint(), m_tailSeg->getEndPoint());
        
        float scaleFactor = dis_headStartToTailEnd == 0
        ? 1
        : dis_headStartToEnd / dis_headStartToTailEnd;
        
        for (int i = 0; i < nseg; i++) {
            LineSegment *seg = m_segList[i];
            
            seg->setStartPoint(
                               m_headSeg->getStartPoint() +
                               ccpMult(seg->getStartPoint() - m_headSeg->getStartPoint(),
                                       scaleFactor));
            
            seg->setEndPoint(m_headSeg->getStartPoint() +
                             ccpMult(seg->getEndPoint() - m_headSeg->getStartPoint(),
                                     scaleFactor));
            
            if (seg->branchID == 1 && seg->father->branchID == 0) {
                // secondary branch root seg
                seg->setBranchEndPoint(m_headSeg->getStartPoint() +
                                       ccpMult(seg->getBranchEndPoint() - m_headSeg->getStartPoint(),
                                               scaleFactor));
            }
        }
        // because segList changed, so must re-generate mesh and glow sprites
        genMesh();
    }
    // update nGeneration
    updateGenerationTimes();
}

void LightningBoltSprite::debugDraw_segWireFrame(float lineWidth)
{
    glLineWidth(lineWidth);

    // draw mesh wireframe
    int nIDtri = (int)m_mesh->IDtriList.size();
    for (int i = 0; i < nIDtri; i++) {
        CIDTriangle &IDtri = m_mesh->IDtriList[i];
        Cv2 &p0 = m_mesh->vlist[IDtri.getvIDByIndex(0)];
        Cv2 &p1 = m_mesh->vlist[IDtri.getvIDByIndex(1)];
        Cv2 &p2 = m_mesh->vlist[IDtri.getvIDByIndex(2)];
        CCPoint vertices[3] = {ccp(p0.x(), p0.y()), ccp(p1.x(), p1.y()),
            ccp(p2.x(), p2.y())};
        ccDrawPoly(vertices, 3, true);
    }
}

void LightningBoltSprite::debugDraw_segLineAndDot(float lineWidth, float pointSize)
{
    // draw segList
    glLineWidth(lineWidth);
    ccPointSize(pointSize);
    int nSeg = (int)m_segList.size();
    for (int i = 0; i < nSeg; i++) {
        LineSegment *seg = m_segList[i];
        ccDrawLine(seg->getStartPoint(), seg->getEndPoint());
        ccDrawPoint(seg->getStartPoint());
        ccDrawPoint(seg->getEndPoint());
    }
}

void LightningBoltSprite::draw()
{
    // draw mesh
    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    
    //----change shader
    assert(m_program);
    this->setShaderProgram(m_program);
    CGLProgramWithUnifos *program =
    (CGLProgramWithUnifos *)this->getShaderProgram();
    ccGLEnable(m_eGLServerState);
    // pass values for cocos2d-x build-in uniforms
    program->use();
    program->setUniformsForBuiltins();
    // pass values for my own uniforms
    float alpha = (float)this->getOpacity() / 255.0 * m_opacityZoom;
    program->passUnifoValue1f("u_opacity", alpha);
    //----enable attributes
    bool isAttribPositionOn = CindexVBO::isEnabledAttribArray_position();
    bool isAttribColorOn = CindexVBO::isEnabledAttribArray_color();
    bool isAttribTexCoordOn = CindexVBO::isEnabledAttribArray_texCoord();
    CindexVBO::enableAttribArray_position(true);
    CindexVBO::enableAttribArray_texCoord(true);
    CindexVBO::enableAttribArray_color(true);
    //----bindTexture
    ccGLBindTexture2D(this->getTexture()->getName());
    //----draw
    m_indexVBO->setPointer_position();
    m_indexVBO->setPointer_texCoord();
    m_indexVBO->setPointer_color();
    m_indexVBO->draw(GL_TRIANGLES);
    //----disable attributes
    CindexVBO::enableAttribArray_position(isAttribPositionOn);
    CindexVBO::enableAttribArray_color(isAttribColorOn);
    CindexVBO::enableAttribArray_texCoord(isAttribTexCoordOn);
    //----unbindTexture
    ccGLBindTexture2D(0);
    
    // debugDraw
    if (m_isDrawDebug) {
        debugDraw_segLineAndDot(0.5, 3);
        //    debugDraw_segWireFrame(1);
    }
    CHECK_GL_ERROR_DEBUG();
}