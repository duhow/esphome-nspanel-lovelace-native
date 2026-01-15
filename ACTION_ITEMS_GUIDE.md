# Action-Only UI Elements

## Overview

As of this version, NSPanel Lovelace Native supports **action-only UI elements** that execute actions when pressed without requiring a Home Assistant entity. This feature allows you to:

- Call Home Assistant services directly from UI buttons
- Execute internal ESPHome actions (scripts, button presses, etc.)
- Create UI controls for services that don't have corresponding entities
- Mix action-only buttons with regular entity-bound buttons in the same card

## Basic Usage

Action-only elements are defined using `type: action` within a card's `entities:` list:

```yaml
nspanel_lovelace:
  cards:
    - type: cardGrid
      title: My Controls
      entities:
        # Regular entity-bound button (existing functionality)
        - entity_id: light.living_room
          name: Light
          
        # NEW: Action-only button
        - type: action
          icon: robot-vacuum
          name: Start Vacuum
          on_press:
            - homeassistant.service:
                service: vacuum.start
                data:
                  entity_id: vacuum.mi_robot
```

## Configuration Options

### Required Fields

- **`type`**: Must be set to `action` to create an action-only element
- **`on_press`**: List of actions to execute when the button is pressed (uses standard ESPHome automation syntax)

### Optional Fields

- **`id`**: Custom identifier for the action item (auto-generated if not provided)
- **`name`**: Display name shown on the button
- **`icon`**: Icon to display (name or hex value, e.g., `robot-vacuum` or `hex:E70C`)

## Examples

### Example 1: Call Home Assistant Service (Vacuum)

```yaml
- type: action
  icon: robot-vacuum
  name: Aspira
  on_press:
    - homeassistant.service:
        service: vacuum.start
        data:
          entity_id: vacuum.mi_robot
```

### Example 2: Call Home Assistant Action with Area Target

```yaml
- type: action
  id: cover_open_all
  icon: chevron-double-up
  name: Open All
  on_press:
    - homeassistant.action:
        action: cover.open_cover
        data:
          target:
            area_id: b973d73bfc3511eab52731eb6f81cdc5
```

### Example 3: Internal ESPHome Button

For internal ESPHome buttons (with `internal: true`) that shouldn't appear in Home Assistant:

```yaml
# In your ESPHome config
button:
  - platform: template
    id: my_internal_button
    name: "Internal Action"
    internal: true
    on_press:
      - logger.log: "Internal button pressed"

# In nspanel_lovelace
- type: action
  icon: gesture-tap-button
  name: Execute
  on_press:
    - button.press: my_internal_button
```

### Example 4: Multiple Actions

Action-only buttons can execute multiple actions in sequence:

```yaml
- type: action
  icon: power
  name: Night Mode
  on_press:
    - homeassistant.service:
        service: light.turn_off
        data:
          entity_id: light.living_room
    - homeassistant.service:
        service: cover.close_cover
        data:
          entity_id: cover.bedroom
    - switch.turn_on: night_mode_switch
```

### Example 5: ESPHome Script

```yaml
script:
  - id: my_script
    then:
      - logger.log: "Script running"
      - delay: 1s
      - logger.log: "Script complete"

# In nspanel_lovelace
- type: action
  icon: script-text
  name: Run Script
  on_press:
    - script.execute: my_script
```

## Complete Example from Issue #54

This example demonstrates using action-only buttons for cover controls that are internal to ESPHome:

```yaml
button:
  - platform: template
    id: cover_wz_open
    name: "Cover WZ Open"
    internal: true
    on_press:
      - cover.open: my_cover
      
  - platform: template
    id: cover_wz_stop
    name: "Cover WZ Stop"
    internal: true
    on_press:
      - cover.stop: my_cover
      
  - platform: template
    id: cover_wz_close
    name: "Cover WZ Close"
    internal: true
    on_press:
      - cover.close: my_cover

nspanel_lovelace:
  cards:
    - type: cardGrid
      title: Rollo WZ
      id: rollo_wz
      entities:
        - type: action
          id: cover_wz_open_action
          icon: chevron-double-up
          name: Auf
          on_press:
            - button.press: cover_wz_open
            
        - type: action
          id: cover_wz_stop_action
          icon: stop
          name: Stop
          on_press:
            - button.press: cover_wz_stop
            
        - type: action
          id: cover_wz_close_action
          icon: chevron-double-down
          name: Zu
          on_press:
            - button.press: cover_wz_close
```

## Supported Actions

Action-only buttons support all standard ESPHome automation actions, including:

- **Home Assistant Services**: `homeassistant.service`, `homeassistant.action`
- **ESPHome Components**: `switch.toggle`, `light.turn_on`, `cover.open`, etc.
- **Scripts**: `script.execute`, `script.stop`
- **Buttons**: `button.press`
- **Logging**: `logger.log`
- **Delays**: `delay`
- **Conditionals**: `if`, `while`
- **And many more**: See [ESPHome Automations](https://esphome.io/guides/automations.html)

## Backward Compatibility

Existing configurations using `entity_id:` continue to work exactly as before. The new `type: action` functionality is completely optional and additive.

```yaml
# These all work without changes:
entities:
  - entity_id: light.bedroom
    name: Bedroom
  - entity_id: switch.fan
    name: Fan
  - entity_id: scene.movie_time
    name: Movie
```

## Visual Appearance

Action-only buttons look identical to regular entity-bound buttons:
- Same icon and label styling
- Same press feedback animation
- Same size and positioning in grids

The only difference is their behavior: they execute actions directly instead of controlling Home Assistant entities.

## Limitations and Notes

1. **No State Display**: Action-only buttons don't show entity state (e.g., on/off status) since they're not bound to an entity. Use regular `entity_id` buttons if you need state feedback.

2. **Supported Card Types**: Action-only buttons work with:
   - `cardGrid`
   - `cardGrid2`
   - `cardEntities`
   - `cardQR`
   - `cardMedia`

3. **No Home Assistant Registration**: Action-only items are not registered as entities in Home Assistant, which means:
   - ✅ No "missing entity" errors
   - ✅ Cleaner entity list in HA
   - ❌ Cannot be controlled from HA (only from the panel)

4. **ID Recommendations**: For better debugging, provide explicit `id:` values for action items instead of relying on auto-generated IDs.

## Troubleshooting

### "Unknown action type" Error

Make sure you're using `type: action` (not `type: button` or other values).

### Actions Not Executing

1. Check ESPHome logs for errors
2. Verify the action syntax matches [ESPHome documentation](https://esphome.io/guides/automations.html)
3. Ensure referenced components (scripts, buttons, etc.) are defined
4. Test actions independently (e.g., call via `button.press` from HA)

### Button Appears But Doesn't Look Right

Ensure you've provided both `name:` and `icon:` fields. Without these, the button may render with defaults.

## Migration from Workarounds

If you were previously using workarounds (like creating fake entities or using `navigate` tricks), you can now replace those with clean action-only buttons:

**Before (workaround with navigate):**
```yaml
- entity_id: navigate.action_card
  name: Do Something
```

**After (clean action-only):**
```yaml
- type: action
  name: Do Something
  on_press:
    - homeassistant.service: ...
```

## Future Enhancements

Potential future improvements (not yet implemented):
- Visual state feedback for action buttons (e.g., pulsing icon during execution)
- Long-press actions
- Conditional button visibility based on entity states
- Button templates for common action patterns

## Related Resources

- Original Feature Request: [Issue #54](https://github.com/olicooper/esphome-nspanel-lovelace-native/issues/54)
- ESPHome Automations: https://esphome.io/guides/automations.html
- Home Assistant Service Calls: https://www.home-assistant.io/docs/scripts/service-calls/
