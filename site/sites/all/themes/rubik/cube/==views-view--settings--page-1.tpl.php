<?php
// content-field.tpl.php

/**
 * @file content-field.tpl.php
 * Default theme implementation to display the value of a field.
 *
 * Available variables:
 * - $node: The node object.
 * - $field: The field array.
 * - $items: An array of values for each item in the field array.
 * - $teaser: Whether this is displayed as a teaser.
 * - $page: Whether this is displayed as a page.
 * - $field_name: The field name.
 * - $field_type: The field type.
 * - $field_name_css: The css-compatible field name.
 * - $field_type_css: The css-compatible field type.
 * - $label: The item label.
 * - $label_display: Position of label display, inline, above, or hidden.
 * - $field_empty: Whether the field has any valid value.
 *
 * Each $item in $items contains:
 * - 'view' - the themed view for that item
 *
 * @see template_preprocess_content_field()
 */
?>

<?php if (!$field_empty) : ?>

<form name="setting-form" method="post" action="/settings">

<div class="column-wrapper clear-block">

    <div class="form-item form-item-labeled">
        <label for="edit-field-mileage-charge-value">Mileage Charge: <span class="form-required" title="This field is required.">*</span></label>
        <input type="text" name="edit-field-mileage-charge-value" class="form-text" value="" />
    </div>

    <div class="form-item form-item-labeled">
        <label for="edit-field-crew-rates-value">Crew Rates: <span class="form-required" title="This field is required.">*</span></label>
        <input type="text" name="field-crew-rates" class="form-text" id="edit-field-crew-rates-value">
    </div>

    <div class="form-item form-item-labeled">
        <label for="edit-field-client-rates-value">Client Rates: <span class="form-required" title="This field is required.">*</span></label>
        <input type="text" name="field-client-rates" class="form-text" id="edit-field-client-rates-value">
    </div>

    <div class="form-item form-item-labeled">
        <div class="buttons">
            <input type="submit" name="set-button" id="set-button" value="Setting" class="form-submit" onclick="setting_button_onclick()" />
        </div>
    </div>

</div>

</form>

<?php endif; ?>