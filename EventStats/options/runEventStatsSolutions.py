import os

from Gaudi.Configuration import INFO
from k4FWCore import IOSvc, ApplicationMgr
from Configurables import EventDataSvc, GeoSvc

io_svc = IOSvc("IOSvc")
io_svc.Input = "/afs/cern.ch/work/a/adevita/public/DD4hepTutorials/simplecalo.root"
io_svc.Output = "output.root"

from Configurables import EventStats
        
eventStats_functional = EventStats("EventStats",
        
    InputCaloHitCollection=["simplecaloRO"],
    EventStats=["TotalDepositedEnergy", "XPosBarycentre", "YPosBarycentre", "ZPosBarycentre",
                 "XPosMax", "YPosMax", "ZPosMax", "XPosMin", "YPosMin", "ZPosMin",
                 "MaxEnergyValue", "MinEnergyValue", "MaxEnergyLayerPosX", 
                 "MaxEnergyLayerPosY", "MaxEnergyLayerPosZ",
                 "MinEnergyLayerPosX", "MinEnergyLayerPosY", "MinEnergyLayerPosZ"],
    plotHistograms=True,
    OutputLevel=INFO
)

app_mgr = ApplicationMgr(
    TopAlg=[eventStats_functional],
    EvtSel='NONE',
    EvtMax=-1,
    ExtSvc=[EventDataSvc("EventDataSvc")],
    StopOnSignal=True,
)