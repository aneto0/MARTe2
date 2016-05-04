$Application1 = {
    Class = RealTimeApplication
    +Functions = {
        Class = ReferenceContainer
        +GAM1 = {
            Class = DummyGAM
            +Input = {
                Class = RealTimeDataDefContainer
                IsInput = true
                IsFinal = true
                +Counter = {
                    Class = RealTimeGenericDataDef
                    Type = uint32
                    Default = 0
                    Path = +DDB.+Counter1
                    IsFinal = true
                }
            }
            +Output = {
                Class = RealTimeDataDefContainer
                IsOutput = true
                IsFinal = true
                +Counter = {
                    Class = RealTimeGenericDataDef
                    Type = uint32
                    Default = 0
                    Path = +DDB.Counter2
                    IsFinal = true
                }
            }
        }
        +GAM2 = {
            Class = DummyGAM
            +Input = {
                Class = RealTimeDataDefContainer
                IsInput = true
                IsFinal = true
                +Counter = {
                    Class = RealTimeGenericDataDef
                    Type = uint32
                    Default = 0
                    Path = +DDB.Counter2
                    IsFinal = true
                }
            }
            +Output = {
                Class = RealTimeDataDefContainer
                IsOutput = true
                IsFinal = true
                +Counter = {
                    Class = RealTimeGenericDataDef
                    Type = uint32
                    Default = 0
                    Path = +DDB.+Counter1
                    IsFinal = true
                }
            }
        }
    }
    +States = {
        Class = ReferenceContainer
        +State1 = {
            Class = RealTimeState
            +Threads = {
                Class = ReferenceContainer
                +Thread1 = {
                    Class = RealTimeThread
                    Functions = {:+Functions.+GAM1 :+Functions.+GAM2}
                }
            }
        }
    }
    +Data = {
        Class = RealTimeDataSourceContainer
        +DDB = {
            Class = RealTimeDataSource
            +Counter1 = {
                Class = SharedDataSource
            }
        }
    }
    +Scheduler = {
        Class = BasicGAMScheduler
    }
}
