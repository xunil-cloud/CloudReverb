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
    0.684854,    // LineDelay
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
    0.342368,    // LineDelay
    0.41500008106231689,    // LineDecay
    0.0,                    // LateDiffusionEnabled
    0.57142859697341919,    // LateDiffusionStages
    (70 - 10) /  90.f,    // LateDiffusionDelay
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
    219 / 500.f,    // TapLength
    1.0,                    // TapGain
    1.0,                    // TapDecay
    1.0,                    // DiffusionEnabled
    0.4285714328289032,     // DiffusionStages
    (34 - 10) /  90.f,    // DiffusionDelay
    0.660000205039978,      // DiffusionFeedback
    0.72727274894714355,    // LineCount
    0.224999,    // LineDelay
    0.794999897480011,      // LineDecay
    1.0,                    // LateDiffusionEnabled
    1.0,                    // LateDiffusionStages
    (30 - 10) /  90.f,    // LateDiffusionDelay
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
    0.762768,    // DryOut
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
    0.584231,      // LineDelay
    0.29499980807304382,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.57142859697341919,    // LateDiffusionStages
    (72 - 10) / 90.f, // LateDiffusionDelay
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
    (41 - 10) / 90.f,    // DiffusionDelay
    0.62500005960464478,    // DiffusionFeedback
    0.63636362552642822,    // LineCount
    0.359784,    // LineDelay
    0.51000005006790161,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.0,                    // LateDiffusionStages
    (66 - 10) / 90.f,    // LateDiffusionDelay
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
    0.684854,    // LineDelay
    0.82999974489212036,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.71428573131561279,    // LateDiffusionStages
    (72 - 10) / 90.f,    // LateDiffusionDelay
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
    219 / 500.f,    // TapLength
    0.87999999523162842,    // TapGain
    1.0,                    // TapDecay
    1.0,                    // DiffusionEnabled
    0.71428573131561279,    // DiffusionStages
    0.335000216960907,      // DiffusionDelay
    0.660000205039978,      // DiffusionFeedback
    0.18181818723678589,    // LineCount
    0.508410,    // LineDelay
    0.29999998211860657,    // LineDecay
    1.0,                    // LateDiffusionEnabled
    0.4285714328289032,     // LateDiffusionStages
    (30 - 10) / 90.f,    // LateDiffusionDelay
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
    (73 - 10) / 90.f,     // DiffusionDelay
    0.5450003147125244,     // DiffusionFeedback
    0.7272727489471436,     // LineCount
    0.684854,     // LineDelay
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
    0.909960,     // LineDelay
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



const static preset AiryClearSpace = {
    0.0,                    // InputMix
    0.045,                  // PreDelay
    0.18,                   // HighPass - gentle low cleanup
    0.92,                   // LowPass - open/bright input tone
    0.50,                   // TapCount
    0.62,                   // TapLength
    0.92,                   // TapGain
    0.58,                   // TapDecay - cleaner early taps
    1.0,                    // DiffusionEnabled
    0.5714285969734192,     // DiffusionStages
    0.34,                   // DiffusionDelay
    0.58,                   // DiffusionFeedback - cleaner early diffusion
    0.8181818127632141,     // LineCount
    0.56,                   // LineDelay
    0.58,                   // LineDecay - clear medium tail
    1.0,                    // LateDiffusionEnabled
    0.4285714328289032,     // LateDiffusionStages
    0.42,                   // LateDiffusionDelay
    0.52,                   // LateDiffusionFeedback - avoids cloudy tail
    0.42,                   // PostLowShelfGain - less low-mid buildup
    0.24,                   // PostLowShelfFrequency
    0.92,                   // PostHighShelfGain - air/clarity
    0.78,                   // PostHighShelfFrequency
    0.90,                   // PostCutoffFrequency - more open top end
    0.28,                   // EarlyDiffusionModAmount
    0.34,                   // EarlyDiffusionModRate
    0.32,                   // LineModAmount
    0.38,                   // LineModRate
    0.24,                   // LateDiffusionModAmount
    0.34,                   // LateDiffusionModRate
    (1150 - 1) * 0.000'001, // TapSeed
    (189 - 1) * 0.000'001,  // DiffusionSeed
    (337 - 1) * 0.000'001,  // DelaySeed
    (501 - 1) * 0.000'001,  // PostDiffusionSeed
    0.45,                   // CrossSeed
    0.86,                   // DryOut
    0.0,                    // PredelayOut
    0.66,                   // EarlyOut
    0.64,                   // MainOut
    1.0,                    // HiPassEnabled
    0.0,                    // LowPassEnabled - keep top end open
    1.0,                    // LowShelfEnabled
    1.0,                    // HighShelfEnabled
    1.0,                    // CutoffEnabled
    1.0,                    // LateStageTap
    1.0                     // Interpolation
};



const static preset TinyBooth = {
    0.0, 0.010, 0.24, 0.78, 0.14, 0.08, 0.62, 0.22,
    1.0, 0.14285715, 0.16, 0.28, 0.09090909, 0.08, 0.10,
    0.0, 0.14285715, 0.12, 0.25, 0.38, 0.18, 0.62, 0.62, 0.78,
    0.08, 0.18, 0.05, 0.16, 0.04, 0.15,
    (101 - 1) * 0.000'001, (151 - 1) * 0.000'001, (211 - 1) * 0.000'001, (271 - 1) * 0.000'001,
    0.12, 0.88, 0.0, 0.48, 0.32, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset SmallStudio = {
    0.0, 0.018, 0.22, 0.82, 0.22, 0.16, 0.70, 0.30,
    1.0, 0.2857143, 0.22, 0.36, 0.18181818, 0.16, 0.18,
    1.0, 0.2857143, 0.20, 0.32, 0.40, 0.20, 0.68, 0.64, 0.80,
    0.12, 0.22, 0.08, 0.20, 0.07, 0.18,
    (137 - 1) * 0.000'001, (199 - 1) * 0.000'001, (263 - 1) * 0.000'001, (331 - 1) * 0.000'001,
    0.18, 0.88, 0.0, 0.55, 0.40, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset WarmRoom = {
    0.0, 0.025, 0.16, 0.72, 0.30, 0.24, 0.76, 0.42,
    1.0, 0.42857143, 0.30, 0.45, 0.27272727, 0.24, 0.28,
    1.0, 0.2857143, 0.28, 0.38, 0.46, 0.26, 0.58, 0.52, 0.70,
    0.16, 0.26, 0.12, 0.24, 0.10, 0.22,
    (173 - 1) * 0.000'001, (239 - 1) * 0.000'001, (307 - 1) * 0.000'001, (379 - 1) * 0.000'001,
    0.25, 0.90, 0.0, 0.60, 0.48, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0
};

const static preset BrightRoom = {
    0.0, 0.020, 0.20, 0.92, 0.32, 0.22, 0.82, 0.38,
    1.0, 0.42857143, 0.26, 0.42, 0.27272727, 0.22, 0.30,
    1.0, 0.2857143, 0.26, 0.36, 0.38, 0.22, 0.88, 0.76, 0.90,
    0.18, 0.30, 0.12, 0.30, 0.12, 0.26,
    (191 - 1) * 0.000'001, (251 - 1) * 0.000'001, (337 - 1) * 0.000'001, (421 - 1) * 0.000'001,
    0.28, 0.86, 0.0, 0.62, 0.50, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset ShortPlate = {
    0.0, 0.012, 0.12, 0.86, 0.42, 0.20, 0.86, 0.52,
    1.0, 0.7142857, 0.24, 0.52, 0.36363636, 0.20, 0.34,
    1.0, 0.5714286, 0.24, 0.46, 0.34, 0.16, 0.82, 0.70, 0.86,
    0.18, 0.36, 0.10, 0.28, 0.10, 0.28,
    (223 - 1) * 0.000'001, (281 - 1) * 0.000'001, (347 - 1) * 0.000'001, (449 - 1) * 0.000'001,
    0.32, 0.84, 0.0, 0.58, 0.54, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset DrumChamber = {
    0.0, 0.030, 0.18, 0.80, 0.46, 0.32, 0.90, 0.56,
    1.0, 0.5714286, 0.36, 0.56, 0.45454545, 0.32, 0.40,
    1.0, 0.42857143, 0.32, 0.50, 0.42, 0.22, 0.74, 0.66, 0.82,
    0.22, 0.34, 0.18, 0.32, 0.16, 0.28,
    (257 - 1) * 0.000'001, (313 - 1) * 0.000'001, (389 - 1) * 0.000'001, (467 - 1) * 0.000'001,
    0.38, 0.86, 0.0, 0.66, 0.58, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset VocalChamber = {
    0.0, 0.040, 0.24, 0.88, 0.38, 0.36, 0.86, 0.48,
    1.0, 0.5714286, 0.34, 0.50, 0.45454545, 0.36, 0.44,
    1.0, 0.42857143, 0.34, 0.46, 0.36, 0.18, 0.86, 0.72, 0.88,
    0.20, 0.30, 0.16, 0.30, 0.14, 0.28,
    (277 - 1) * 0.000'001, (337 - 1) * 0.000'001, (401 - 1) * 0.000'001, (491 - 1) * 0.000'001,
    0.40, 0.88, 0.0, 0.70, 0.60, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset MediumHallClear = {
    0.0, 0.055, 0.20, 0.90, 0.50, 0.48, 0.88, 0.56,
    1.0, 0.5714286, 0.42, 0.58, 0.54545455, 0.48, 0.52,
    1.0, 0.5714286, 0.42, 0.54, 0.40, 0.22, 0.90, 0.76, 0.90,
    0.24, 0.34, 0.22, 0.36, 0.18, 0.32,
    (307 - 1) * 0.000'001, (367 - 1) * 0.000'001, (431 - 1) * 0.000'001, (503 - 1) * 0.000'001,
    0.44, 0.84, 0.0, 0.68, 0.68, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset WideHall = {
    0.08, 0.070, 0.18, 0.92, 0.56, 0.58, 0.88, 0.62,
    1.0, 0.7142857, 0.48, 0.62, 0.63636364, 0.58, 0.60,
    1.0, 0.5714286, 0.48, 0.58, 0.42, 0.24, 0.92, 0.78, 0.92,
    0.26, 0.36, 0.28, 0.38, 0.22, 0.34,
    (331 - 1) * 0.000'001, (397 - 1) * 0.000'001, (461 - 1) * 0.000'001, (541 - 1) * 0.000'001,
    0.55, 0.82, 0.0, 0.70, 0.74, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset DeepHall = {
    0.10, 0.085, 0.16, 0.82, 0.60, 0.66, 0.84, 0.70,
    1.0, 0.7142857, 0.54, 0.66, 0.72727273, 0.66, 0.68,
    1.0, 0.7142857, 0.54, 0.62, 0.50, 0.28, 0.76, 0.66, 0.82,
    0.28, 0.32, 0.34, 0.36, 0.26, 0.32,
    (353 - 1) * 0.000'001, (421 - 1) * 0.000'001, (487 - 1) * 0.000'001, (563 - 1) * 0.000'001,
    0.60, 0.80, 0.0, 0.66, 0.80, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0
};

const static preset CathedralAir = {
    0.12, 0.110, 0.22, 0.94, 0.66, 0.78, 0.86, 0.76,
    1.0, 0.8571429, 0.62, 0.70, 0.81818182, 0.78, 0.76,
    1.0, 0.8571429, 0.62, 0.68, 0.38, 0.20, 0.96, 0.82, 0.94,
    0.30, 0.34, 0.38, 0.40, 0.30, 0.36,
    (379 - 1) * 0.000'001, (449 - 1) * 0.000'001, (523 - 1) * 0.000'001, (601 - 1) * 0.000'001,
    0.65, 0.78, 0.0, 0.64, 0.86, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset CinematicBloom = {
    0.16, 0.125, 0.18, 0.90, 0.72, 0.84, 0.88, 0.82,
    1.0, 0.8571429, 0.66, 0.72, 0.90909091, 0.84, 0.82,
    1.0, 0.8571429, 0.66, 0.72, 0.46, 0.24, 0.88, 0.76, 0.92,
    0.34, 0.32, 0.42, 0.36, 0.34, 0.34,
    (401 - 1) * 0.000'001, (467 - 1) * 0.000'001, (547 - 1) * 0.000'001, (631 - 1) * 0.000'001,
    0.70, 0.76, 0.0, 0.62, 0.90, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset InfiniteChamber = {
    0.20, 0.140, 0.20, 0.96, 0.78, 0.92, 0.90, 0.86,
    1.0, 1.0, 0.72, 0.76, 1.0, 0.92, 0.88,
    1.0, 1.0, 0.72, 0.76, 0.42, 0.22, 0.98, 0.84, 0.96,
    0.38, 0.36, 0.48, 0.38, 0.40, 0.36,
    (431 - 1) * 0.000'001, (503 - 1) * 0.000'001, (587 - 1) * 0.000'001, (673 - 1) * 0.000'001,
    0.76, 0.72, 0.0, 0.58, 0.94, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset GalacticBloom = {
    0.22, 0.160, 0.24, 0.98, 0.84, 1.0, 0.92, 0.90,
    1.0, 1.0, 0.76, 0.78, 1.0, 1.0, 0.90,
    1.0, 1.0, 0.76, 0.78, 0.36, 0.18, 1.0, 0.88, 0.98,
    0.44, 0.34, 0.54, 0.40, 0.48, 0.38,
    (463 - 1) * 0.000'001, (541 - 1) * 0.000'001, (619 - 1) * 0.000'001, (701 - 1) * 0.000'001,
    0.82, 0.70, 0.0, 0.54, 0.98, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset NebulaDrift = {
    0.26, 0.180, 0.20, 0.88, 0.88, 1.0, 0.88, 0.92,
    1.0, 1.0, 0.80, 0.82, 1.0, 0.95, 0.92,
    1.0, 1.0, 0.80, 0.82, 0.52, 0.30, 0.78, 0.72, 0.90,
    0.50, 0.26, 0.60, 0.32, 0.54, 0.30,
    (487 - 1) * 0.000'001, (563 - 1) * 0.000'001, (647 - 1) * 0.000'001, (733 - 1) * 0.000'001,
    0.86, 0.68, 0.0, 0.50, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0
};

const static preset CosmicShimmer = {
    0.28, 0.200, 0.28, 1.0, 0.92, 1.0, 0.94, 0.88,
    1.0, 1.0, 0.84, 0.76, 1.0, 0.88, 0.86,
    1.0, 1.0, 0.84, 0.74, 0.30, 0.14, 1.0, 0.92, 1.0,
    0.58, 0.42, 0.66, 0.48, 0.62, 0.44,
    (509 - 1) * 0.000'001, (599 - 1) * 0.000'001, (683 - 1) * 0.000'001, (769 - 1) * 0.000'001,
    0.90, 0.66, 0.0, 0.56, 0.96, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset FrozenOrbit = {
    0.18, 0.150, 0.34, 0.98, 0.78, 0.86, 0.82, 0.72,
    1.0, 0.8571429, 0.70, 0.58, 0.90909091, 0.82, 0.74,
    1.0, 0.8571429, 0.70, 0.62, 0.28, 0.12, 0.96, 0.86, 0.96,
    0.46, 0.22, 0.52, 0.28, 0.46, 0.24,
    (541 - 1) * 0.000'001, (617 - 1) * 0.000'001, (709 - 1) * 0.000'001, (797 - 1) * 0.000'001,
    0.78, 0.70, 0.0, 0.50, 0.88, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static preset BlackHoleTail = {
    0.34, 0.240, 0.12, 0.74, 1.0, 1.0, 0.76, 1.0,
    1.0, 1.0, 0.88, 0.86, 1.0, 1.0, 1.0,
    1.0, 1.0, 0.88, 0.88, 0.62, 0.36, 0.64, 0.58, 0.76,
    0.44, 0.18, 0.70, 0.22, 0.66, 0.20,
    (571 - 1) * 0.000'001, (641 - 1) * 0.000'001, (727 - 1) * 0.000'001, (823 - 1) * 0.000'001,
    0.92, 0.64, 0.0, 0.42, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0
};

const static preset StarlightPlate = {
    0.14, 0.090, 0.26, 0.98, 0.68, 0.62, 0.94, 0.66,
    1.0, 0.8571429, 0.58, 0.62, 0.72727273, 0.62, 0.58,
    1.0, 0.7142857, 0.58, 0.58, 0.32, 0.16, 0.98, 0.84, 0.96,
    0.36, 0.40, 0.38, 0.44, 0.36, 0.40,
    (601 - 1) * 0.000'001, (677 - 1) * 0.000'001, (751 - 1) * 0.000'001, (839 - 1) * 0.000'001,
    0.66, 0.76, 0.0, 0.62, 0.82, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};


const static preset EventHorizon = {
    0.38, 0.280, 0.22, 0.98, 1.0, 1.0, 0.92, 0.96,
    1.0, 1.0, 0.92, 0.82, 1.0, 1.0, 0.96,
    1.0, 1.0, 0.92, 0.86, 0.40, 0.20, 1.0, 0.92, 1.0,
    0.68, 0.36, 0.74, 0.38, 0.72, 0.34,
    (661 - 1) * 0.000'001, (733 - 1) * 0.000'001, (811 - 1) * 0.000'001, (887 - 1) * 0.000'001,
    1.0, 0.60, 0.0, 0.48, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

const static struct preset presets[30] = {ChorusDelay, DullEchoes,        Hyperplane,
                                          MediumSpace, NoiseInTheHallway, RubiKaFields,
                                          SmallRoom,   The90sAreBack,     ThroughTheLookingGlass,
                                          AiryClearSpace,
                                          TinyBooth, SmallStudio, WarmRoom, BrightRoom, ShortPlate,
                                          DrumChamber, VocalChamber, MediumHallClear, WideHall, DeepHall,
                                          CathedralAir, CinematicBloom, InfiniteChamber, GalacticBloom, NebulaDrift,
                                          CosmicShimmer, FrozenOrbit, BlackHoleTail, StarlightPlate,
                                          EventHorizon};

} // namespace cloudPresets
