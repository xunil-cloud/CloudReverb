namespace cloudPresets
{
struct preset
{
    double InputMix;
    double PreDelay;
    double HighPass;
    double LowPass;
    double TapCount;
    double TapLength;
    double TapGain;
    double TapDecay;
    double DiffusionEnabled;
    double DiffusionStages;
    double DiffusionDelay;
    double DiffusionFeedback;
    double LineCount;
    double LineDelay;
    double LineDecay;
    double LateDiffusionEnabled;
    double LateDiffusionStages;
    double LateDiffusionDelay;
    double LateDiffusionFeedback;
    double PostLowShelfGain;
    double PostLowShelfFrequency;
    double PostHighShelfGain;
    double PostHighShelfFrequency;
    double PostCutoffFrequency;
    double EarlyDiffusionModAmount;
    double EarlyDiffusionModRate;
    double LineModAmount;
    double LineModRate;
    double LateDiffusionModAmount;
    double LateDiffusionModRate;
    double TapSeed;
    double DiffusionSeed;
    double DelaySeed;
    double PostDiffusionSeed;
    double CrossSeed;
    double DryOut;
    double PredelayOut;
    double EarlyOut;
    double MainOut;
    double HiPassEnabled;
    double LowPassEnabled;
    double LowShelfEnabled;
    double HighShelfEnabled;
    double CutoffEnabled;
    double LateStageTap;
    double Interpolation;
};

const static preset ChorusDelay = {
    0.0,                    // InputMix
    0.070000000298023224,   // PreDelay
    0.0,                    // HighPass
    0.29000008106231689,    // LowPass
                            // 0.36499997973442078,
    0.3469387755102041,     // TapCount
    1.0,                    // TapLength
    1.0,                    // TapGain
    0.86500012874603271,    // TapDecay
    1.0,                    // DiffusionEnabled
    0.4285714328289032,     // DiffusionStages
    0.43500006198883057,    // DiffusionDelay
    0.725000262260437,      // DiffusionFeedback
    1.0,                    // LineCount
    0.68499988317489624,    // LineDelay
    0.68000012636184692,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.28571429848670959,    // LateDiffusionStages
    0.54499995708465576,    // LateDiffusionDelay
    0.65999996662139893,    // LateDiffusionFeedback
    0.5199999213218689,     // PostLowShelfGain
    0.31499990820884705,    // PostLowShelfFrequency
    0.83500003814697266,    // PostHighShelfGain
    0.73000013828277588,    // PostHighShelfFrequency
    0.73499983549118042,    // PostCutoffFrequency
    0.50000005960464478,    // EarlyDiffusionModAmount
    0.42500010132789612,    // EarlyDiffusionModRate
    0.59000003337860107,    // LineModAmount
    0.46999993920326233,    // LineModRate
    0.619999885559082,      // LateDiffusionModAmount
    0.42500019073486328,    // LateDiffusionModRate
    0.001149,               // TapSeed
    0.000188,               // DiffusionSeed
    0.000336,               // DelaySeed
                            // 0.00050099997315555811,
    0.0005,                 // PostDiffusionSeed
    0.0,                    // CrossSeed
                            // 0.94499987363815308,
    0.9012339233603366,     // DryOut
    0.0,                    // PredelayOut
                            // 0.77999997138977051,
    0.6242610511276194,     // EarlyOut
                            // 0.74500006437301636,
    0.570361946138122,      // MainOut
    0.0,                    // HiPassEnabled
    0.0,                    // LowPassEnabled
    0.0,                    // LowShelfEnabled
    0.0,                    // HighShelfEnabled
    1.0,                    // CutoffEnabled
    1.0,                    // LateStageTap
    0.0                     // Interpolation
};

const static preset DullEchoes = {
    0.0,                    // InputMix
    0.070000000298023224,   // PreDelay
    0.0,                    // HighPass
    0.29000008106231689,    // LowPass
    0.346939,               // TapCount
    1.0,                    // TapLength
    0.83499991893768311,    // TapGain
    0.86500012874603271,    // TapDecay
    1.0,                    // DiffusionEnabled
    0.4285714328289032,     // DiffusionStages
    0.43500006198883057,    // DiffusionDelay
    0.725000262260437,      // DiffusionFeedback
    1.0,                    // LineCount
    0.34500002861022949,    // LineDelay
    0.41500008106231689,    // LineDecay
    0.0,                    // LateDiffusionEnabled
    0.57142859697341919,    // LateDiffusionStages
    0.66499996185302734,    // LateDiffusionDelay
    0.61000001430511475,    // LateDiffusionFeedback
    0.5199999213218689,     // PostLowShelfGain
    0.31499990820884705,    // PostLowShelfFrequency
    0.83500003814697266,    // PostHighShelfGain
    0.73000013828277588,    // PostHighShelfFrequency
    0.73499983549118042,    // PostCutoffFrequency
    0.25499999523162842,    // EarlyDiffusionModAmount
    0.3250001072883606,     // EarlyDiffusionModRate
    0.33500000834465027,    // LineModAmount
    0.26999998092651367,    // LineModRate
    0.13499975204467773,    // LateDiffusionModAmount
    0.27500006556510925,    // LateDiffusionModRate
    (1150 - 1) * 0.000'001,  // TapSeed
    (189 - 1) * 0.000'001, // DiffusionSeed
    (273 - 1) * 0.000'001,  // DelaySeed
    (501 - 1) * 0.000'001, // PostDiffusionSeed
    0.5,                    // CrossSeed
    1.0,                    // DryOut
    0.0,                    // PredelayOut
    0.624262,    // EarlyOut
    0.570362,    // MainOut
    0.0,                    // HiPassEnabled
    1.0,                    // LowPassEnabled
    0.0,                    // LowShelfEnabled
    0.0,                    // HighShelfEnabled
    1.0,                    // CutoffEnabled
    0.0,                    // LateStageTap
    1.0                     // Interpolation
};

const static preset Hyperplane = {
    0.1549999862909317,     // InputMix
    0.0,                    // PreDelay
    0.57999998331069946,    // HighPass
    0.9100000262260437,     // LowPass
    0.41499990224838257,    // TapCount
    0.43999996781349182,    // TapLength
    1.0,                    // TapGain
    1.0,                    // TapDecay
    1.0,                    // DiffusionEnabled
    0.4285714328289032,     // DiffusionStages
    0.27500024437904358,    // DiffusionDelay
    0.660000205039978,      // DiffusionFeedback
    0.72727274894714355,    // LineCount
    0.22500017285346985,    // LineDelay
    0.794999897480011,      // LineDecay
    1.0,                    // LateDiffusionEnabled
    1.0,                    // LateDiffusionStages
    0.22999951243400574,    // LateDiffusionDelay
    0.59499990940093994,    // LateDiffusionFeedback
    0.95999979972839355,    // PostLowShelfGain
    0.23999994993209839,    // PostLowShelfFrequency
    0.97500002384185791,    // PostHighShelfGain
    0.78499996662139893,    // PostHighShelfFrequency
    0.87999981641769409,    // PostCutoffFrequency
    0.13499999046325684,    // EarlyDiffusionModAmount
    0.29000008106231689,    // EarlyDiffusionModRate
    0.53999996185302734,    // LineModAmount
    0.44999989867210388,    // LineModRate
    0.15999998152256012,    // LateDiffusionModAmount
    0.56000012159347534,    // LateDiffusionModRate
    (485 - 1) * 0.000'001,  // TapSeed
    (208 - 1) * 0.000'001, // DiffusionSeed
    (347 - 1) * 0.000'001,  // DelaySeed
    (372 - 1) * 0.000'001, // PostDiffusionSeed
    0.800000011920929,      // CrossSeed
    0.86500018835067749,    // DryOut
    0.0,                    // PredelayOut
    0.688190,     // EarlyOut
    0.647956,    // MainOut
    1.0,                    // HiPassEnabled
    1.0,                    // LowPassEnabled
    1.0,                    // LowShelfEnabled
    1.0,                    // HighShelfEnabled
    1.0,                    // CutoffEnabled
    1.0,                    // LateStageTap
    0.0                     // Interpolation
};

const static preset MediumSpace = {
    0.0,                    // InputMix
    0.0,                    // PreDelay
    0.0,                    // HighPass
    0.63999992609024048,    // LowPass
    0.51999980211257935,    // TapCount
    0.26499992609024048,    // TapLength
    0.69499999284744263,    // TapGain
    1.0,                    // TapDecay
    1.0,                    // DiffusionEnabled
    0.8571428656578064,     // DiffusionStages
    0.5700000524520874,     // DiffusionDelay
    0.76000010967254639,    // DiffusionFeedback
    0.18181818723678589,    // LineCount
    0.585000216960907,      // LineDelay
    0.29499980807304382,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.57142859697341919,    // LateDiffusionStages
    0.69499951601028442,    // LateDiffusionDelay
    0.71499985456466675,    // LateDiffusionFeedback
    0.87999987602233887,    // PostLowShelfGain
    0.19499993324279785,    // PostLowShelfFrequency
    0.72000008821487427,    // PostHighShelfGain
    0.520000159740448,      // PostHighShelfFrequency
    0.79999983310699463,    // PostCutoffFrequency
    0.13499999046325684,    // EarlyDiffusionModAmount
    0.26000010967254639,    // EarlyDiffusionModRate
    0.054999928921461105,   // LineModAmount
    0.21499986946582794,    // LineModRate
    0.17999963462352753,    // LateDiffusionModAmount
    0.38000011444091797,    // LateDiffusionModRate
    (301 - 1) * 0.000'001,  // TapSeed
    (189 - 1) * 0.000'001, // DiffusionSeed
    (161 - 1) * 0.000'001,  // DelaySeed
    (501 - 1) * 0.000'001, // PostDiffusionSeed
    0.7850000262260437,     // CrossSeed
    1.0,                    // DryOut
    0.0,                    // PredelayOut
    0.504148,      // EarlyOut
    0.729296,    // MainOut
    0.0,                    // HiPassEnabled
    1.0,                    // LowPassEnabled
    1.0,                    // LowShelfEnabled
    0.0,                    // HighShelfEnabled
    1.0,                    // CutoffEnabled
    1.0,                    // LateStageTap
    1.0                     // Interpolation
};

const static preset NoiseInTheHallway = {
    0.0,                    // InputMix
    0.0,                    // PreDelay
    0.0,                    // HighPass
    0.60999995470047,       // LowPass
    1.0,                    // TapCount
    1.0,                    // TapLength
    0.0,                    // TapGain
    0.830000102519989,      // TapDecay
    1.0,                    // DiffusionEnabled
    0.28571429848670959,    // DiffusionStages
    0.35499998927116394,    // DiffusionDelay
    0.62500005960464478,    // DiffusionFeedback
    0.63636362552642822,    // LineCount
    0.36000004410743713,    // LineDelay
    0.51000005006790161,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.0,                    // LateDiffusionStages
    0.62999987602233887,    // LateDiffusionDelay
    0.49000000953674316,    // LateDiffusionFeedback
    0.0,                    // PostLowShelfGain
    0.0,                    // PostLowShelfFrequency
    0.77499985694885254,    // PostHighShelfGain
    0.58000004291534424,    // PostHighShelfFrequency
    0.0,                    // PostCutoffFrequency
    0.0,                    // EarlyDiffusionModAmount
    0.0,                    // EarlyDiffusionModRate
    0.0,                    // LineModAmount
    0.0,                    // LineModRate
    0.0,                    // LateDiffusionModAmount
    0.0,                    // LateDiffusionModRate
    (114 - 1) * 0.000'001,  // TapSeed
    (156 - 1) * 0.000'001, // DiffusionSeed
    (181 - 1) * 0.000'001,  // DelaySeed
    (85 - 1) * 0.000'001, // PostDiffusionSeed
    1.0,                    // CrossSeed
    0.0,                    // DryOut
    0.0,                    // PredelayOut
    0.428406,    // EarlyOut
    0.408810,    // MainOut
    0.0,                    // HiPassEnabled
    1.0,                    // LowPassEnabled
    0.0,                    // LowShelfEnabled
    1.0,                    // HighShelfEnabled
    0.0,                    // CutoffEnabled
    0.0,                    // LateStageTap
    1.0                     // Interpolation
};
const static preset RubiKaFields = {
    0.32499998807907104,    // InputMix
    0.0,                    // PreDelay
    0.0,                    // HighPass
    0.8899998664855957,     // LowPass
    0.51999980211257935,    // TapCount
    1.0,                    // TapLength
    0.90000003576278687,    // TapGain
    1.0,                    // TapDecay
    1.0,                    // DiffusionEnabled
    0.8571428656578064,     // DiffusionStages
    0.5700000524520874,     // DiffusionDelay
    0.76000010967254639,    // DiffusionFeedback
    0.27272728085517883,    // LineCount
    0.68500018119812012,    // LineDelay
    0.82999974489212036,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.71428573131561279,    // LateDiffusionStages
    0.69499951601028442,    // LateDiffusionDelay
    0.71499985456466675,    // LateDiffusionFeedback
    0.87999987602233887,    // PostLowShelfGain
    0.19499993324279785,    // PostLowShelfFrequency
    0.72000008821487427,    // PostHighShelfGain
    0.520000159740448,      // PostHighShelfFrequency
    0.79999983310699463,    // PostCutoffFrequency
    0.13499999046325684,    // EarlyDiffusionModAmount
    0.26000010967254639,    // EarlyDiffusionModRate
    0.054999928921461105,   // LineModAmount
    0.21499986946582794,    // LineModRate
    0.32499963045120239,    // LateDiffusionModAmount
    0.35500010848045349,    // LateDiffusionModRate
    (301 - 1) * 0.000'001,  // TapSeed
    (189 - 1) * 0.000'001, // DiffusionSeed
    (161 - 1) * 0.000'001,  // DelaySeed
    (501 - 1) * 0.000'001, // PostDiffusionSeed
    0.43000003695487976,    // CrossSeed
    0.796728,    // DryOut
    0.0,                    // PredelayOut
    0.0,                    // EarlyOut
    0.839817,    // MainOut
    0.0,                    // HiPassEnabled
    0.0,                    // LowPassEnabled
    0.0,                    // LowShelfEnabled
    0.0,                    // HighShelfEnabled
    1.0,                    // CutoffEnabled
    1.0,                    // LateStageTap
    0.0                     // Interpolation
};

const static preset SmallRoom = {
    0.0,                    // InputMix
    0.0,                    // PreDelay
    0.0,                    // HighPass
    0.755000114440918,      // LowPass
    0.41499990224838257,    // TapCount
    0.43999996781349182,    // TapLength
    0.87999999523162842,    // TapGain
    1.0,                    // TapDecay
    1.0,                    // DiffusionEnabled
    0.71428573131561279,    // DiffusionStages
    0.335000216960907,      // DiffusionDelay
    0.660000205039978,      // DiffusionFeedback
    0.18181818723678589,    // LineCount
    0.51000016927719116,    // LineDelay
    0.29999998211860657,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.4285714328289032,     // LateDiffusionStages
    0.22999951243400574,    // LateDiffusionDelay
    0.59499990940093994,    // LateDiffusionFeedback
    0.87999987602233887,    // PostLowShelfGain
    0.19499993324279785,    // PostLowShelfFrequency
    0.875,                  // PostHighShelfGain
    0.59000009298324585,    // PostHighShelfFrequency
    0.79999983310699463,    // PostCutoffFrequency
    0.13499999046325684,    // EarlyDiffusionModAmount
    0.29000008106231689,    // EarlyDiffusionModRate
    0.18999995291233063,    // LineModAmount
    0.22999987006187439,    // LineModRate
    0.1249999925494194,     // LateDiffusionModAmount
    0.28500008583068848,    // LateDiffusionModRate
    (485 - 1) * 0.000'001,  // TapSeed
    (208 - 1) * 0.000'001, // DiffusionSeed
    (335 - 1) * 0.000'001,  // DelaySeed
    (372 - 1) * 0.000'001, // PostDiffusionSeed
    0.42500001192092896,    // CrossSeed
    1.0,                    // DryOut
    0.0,                    // PredelayOut
    0.754354,     // EarlyOut
    0.831145,    // MainOut
    0.0,                    // HiPassEnabled
    1.0,                    // LowPassEnabled
    0.0,                    // LowShelfEnabled
    0.0,                    // HighShelfEnabled
    0.0,                    // CutoffEnabled
    1.0,                    // LateStageTap
    1.0                     // Interpolation
};

const static preset The90sAreBack{
    0,                      // InputMix
    0,                      // PreDelay
    0,                      // HighPass
    0.6750001311302185,     // LowPass
    0,                      // TapCount
    1,                      // TapLength
    0,                      // TapGain
    0.8650001287460327,     // TapDecay
    1,                      // DiffusionEnabled
    0.5714285969734192,     // DiffusionStages
    0.7100000381469727,     // DiffusionDelay
    0.5450003147125244,     // DiffusionFeedback
    0.7272727489471436,     // LineCount
    0.6849998831748962,     // LineDelay
    0.6300000548362732,     // LineDecay
    0,                      // LateDiffusionEnabled
    0.2857142984867096,     // LateDiffusionStages
    0.5449999570846558,     // LateDiffusionDelay
    0.6599999666213989,     // LateDiffusionFeedback
    0.5199999213218689,     // PostLowShelfGain
    0.31499990820884705,    // PostLowShelfFrequency
    0.8349999189376831,     // PostHighShelfGain
    0.705000102519989,      // PostHighShelfFrequency
    0.7349998354911804,     // PostCutoffFrequency
    0.824999988079071,      // EarlyDiffusionModAmount
    0.4050004780292511,     // EarlyDiffusionModRate
    0.6300000548362732,     // LineModAmount
    0.3199999928474426,     // LineModRate
    0.619999885559082,      // LateDiffusionModAmount
    0.30000022053718567,    // LateDiffusionModRate
    (1150 - 1) * 0.000'001,  // TapSeed
    (189 - 1) * 0.000'001, // DiffusionSeed
    (337 - 1) * 0.000'001,  // DelaySeed
    (501 - 1) * 0.000'001, // PostDiffusionSeed
    0.7950000166893005,     // CrossSeed
    0.901234,     // DryOut
    0,                      // PredelayOut
    0.540487,     // EarlyOut
    0.377209,     // MainOut
    0,                      // HiPassEnabled
    1,                      // LowPassEnabled
    0,                      // LowShelfEnabled
    1,                      // HighShelfEnabled
    0,                      // CutoffEnabled
    1,                      // LateStageTap
    1                       // Interpolation
};

const static preset ThroughTheLookingGlass = {
    0.0,                    // InputMix
    0.0,                    // PreDelay
    0.0,                    // HighPass
    0.74000012874603271,    // LowPass
    1.0,                    // TapCount
    1.0,                    // TapLength
    1.0,                    // TapGain
    0.71000003814697266,    // TapDecay
    1.0,                    // DiffusionEnabled
    1.0,                    // DiffusionStages
    0.65999996662139893,    // DiffusionDelay
    0.76000010967254639,    // DiffusionFeedback
    1.0,                    // LineCount
    0.9100002646446228,     // LineDelay
    0.80999958515167236,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    1.0,                    // LateDiffusionStages
    0.71499955654144287,    // LateDiffusionDelay
    0.71999979019165039,    // LateDiffusionFeedback
    0.87999987602233887,    // PostLowShelfGain
    0.19499993324279785,    // PostLowShelfFrequency
    0.72000008821487427,    // PostHighShelfGain
    0.520000159740448,      // PostHighShelfFrequency
    0.7150002121925354,     // PostCutoffFrequency
    0.41999998688697815,    // EarlyDiffusionModAmount
    0.30500012636184692,    // EarlyDiffusionModRate
    0.4649999737739563,     // LineModAmount
    0.3199998140335083,     // LineModRate
    0.40999993681907654,    // LateDiffusionModAmount
    0.31500011682510376,    // LateDiffusionModRate
    (301 - 1) * 0.000'001,  // TapSeed
    (189 - 1) * 0.000'001, // DiffusionSeed
    (161 - 1) * 0.000'001,  // DelaySeed
    (501 - 1) * 0.000'001, // PostDiffusionSeed
    1.0,                    // CrossSeed
    0.0,                    // DryOut
    0.0,                    // PredelayOut
    0.0,                    // EarlyOut
    0.918997,    // MainOut
    0.0,                    // HiPassEnabled
    1.0,                    // LowPassEnabled
    0.0,                    // LowShelfEnabled
    0.0,                    // HighShelfEnabled
    1.0,                    // CutoffEnabled
    1.0,                    // LateStageTap
    1.0                     // Interpolation
};

const static struct preset presets[9] = {ChorusDelay, DullEchoes,        Hyperplane,
                                         MediumSpace, NoiseInTheHallway, RubiKaFields,
                                         SmallRoom,   The90sAreBack,     ThroughTheLookingGlass};

} // namespace cloudPresets
