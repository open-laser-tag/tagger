
https://mermaidjs.github.io/mermaid-live-editor/#/edit/eyJjb2RlIjoic2VxdWVuY2VEaWFncmFtXG4gICAgcGFydGljaXBhbnQgVHJpZ2dlclxuICAgIHBhcnRpY2lwYW50IEVTUDMyXG4gICAgcGFydGljaXBhbnQgQmx1ZXRvb3RoIE1vZHVsZVxuICAgIHBhcnRpY2lwYW50IEluZmFycmVkIE1vZHVsZVxuICAgIGFjdGl2YXRlIEVTUDMyXG4gICAgVHJpZ2dlci0-PkVTUDMyOiBUcmlnZ2VyIFByZXNzIERldGVjdGVkXG4gICAgRVNQMzItPj5CbHVldG9vdGggTW9kdWxlOiBUcmlnZ2VyUHJlc3NlZFxuICAgXG4gICAgQmx1ZXRvb3RoIE1vZHVsZSAtLT4-IEVTUDMyOiBTaG9vdENvbW1hbmQgKyBQbGF5ZXJEYXRhXG4gICBcbiAgICBFU1AzMi0-PkluZmFycmVkIE1vZHVsZTogU2VuZCBEYXRhXG4gICAgSW5mYXJyZWQgTW9kdWxlIC0tPj4gRVNQMzI6IFN1Y2NzZXNzXG4gICAgZGVhY3RpdmF0ZSBFU1AzMlxuIiwibWVybWFpZCI6eyJ0aGVtZSI6ImRlZmF1bHQifX0

sequenceDiagram
    participant Trigger
    participant ESP32
    participant Bluetooth Module
    participant Infarred Module
    activate ESP32
    Trigger->>ESP32: Trigger Press Detected
    ESP32->>Bluetooth Module: TriggerPressed
   
    Bluetooth Module -->> ESP32: ShootCommand + PlayerData
   
    ESP32->>Infarred Module: Send Data
    Infarred Module -->> ESP32: Succsess
    deactivate ESP32
